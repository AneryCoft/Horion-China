#include "FollowPathModule.h"

FollowPathModule::FollowPathModule() : IModule(0x0, Category::MOVEMENT, "Follows joe paths.") {
}

const char* FollowPathModule::getModuleName() {
	return "FollowPath";
}

void FollowPathModule::startSearch(vec3_ti startNode, C_BlockSource* region, float searchTimeout, std::function<void(bool, Path)> callback) {
	if (pathFinder) {
		clientMessageF("Already searching!");
		return;
	}
	pathFinder = std::make_shared<PathFinder>(startNode, region, goal);
	pathFinder->pathSearchTimeout = searchTimeout;
	auto ref = pathFinder;
	auto tempPath = pathFinder->findPath();
	pathFinder.reset();
	if (tempPath.getNumSegments() == 0 || !isEnabled()) {
		callback(false, tempPath);
		return;
	}
	callback(true, tempPath);
}

void FollowPathModule::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.getLocalPlayer()->isAlive()) {
		setEnabled(false);
		return;
	}
	if (!goal) {
		clientMessageF("Setting goal in command");
		setEnabled(false);
		return;
	}
	shouldStartSearch = true;
}

void FollowPathModule::onDisable() {
	shouldStartSearch = false;
	if (pathFinder)
		pathFinder->terminateSearch = true;
	engageDelay = -1;

	pathFinder.reset();
	path.reset();
	nextPath.reset();
	movementController.reset();
	goal.reset();
}

void FollowPathModule::onTick(C_GameMode* mode) {
	if (!shouldStartSearch)
		return;
	shouldStartSearch = false;

	C_LocalPlayer* player = g_Data.getLocalPlayer();
	auto pPos = player->eyePos0;
	vec3_ti startNode((int)floorf(pPos.x), (int)roundf(pPos.y - 1.62f), (int)floorf(pPos.z));

	startSearch(startNode, player->region, 0.5f, [&](bool succeeded, Path tempPath) {
		if (!succeeded) {
			clientMessageF("%sCould not find a path!", RED);
			path.reset();
			movementController.reset();
			setEnabled(false);
			engageDelay = -1;
			return;
		}

		clientMessageF("%sFound %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");

		if (tempPath.isIncomplete1()) {
			tempPath.cutoff(0.9f);
		}
		engageDelay = 10;

		path = std::make_shared<Path>(tempPath.getAllSegments(), tempPath.isIncomplete1());
		movementController = std::make_unique<PathMovementController>(path);
		});
}

void FollowPathModule::onMove(C_MoveInputHandler* handler) {
	if (movementController) {
		movementController->step(g_Data.getLocalPlayer(), g_Data.getClientInstance()->getMoveTurnInput());
		if (engageDelay > 0)
			engageDelay--;

		if (movementController->isDone()) {
			if (movementController->getCurrentPath()->isIncomplete1()) {
				// Replace with next path if it exists
				if (nextPath && !pathFinder) {
					path = nextPath;
					nextPath.reset();
					movementController = std::make_unique<PathMovementController>(path);
				}
				else if (!pathFinder) {
					setEnabled(false);
				}
				else if (g_Data.getLocalPlayer()->isInWater()) {
					handler->isJumping = true;
				}
			}
		}
		else if (!pathFinder && engageDelay == 0 && path && path->isIncomplete1() && !nextPath) {
			engageDelay = 10;

			// Estimate time to completion
			auto curPath = movementController->getCurrentPath();
			float timeSpent = 0;
			if (curPath->getNumSegments() == 0) {
				setEnabled(false);
				return;
			}
			for (size_t i = curPath->getNumSegments() - 1; i > movementController->getCurrentPathSegment(); i--) {
				auto cur = curPath->getSegment(i);
				timeSpent += cur.getCost();
				if (timeSpent > 11)
					break;
			}

			if (timeSpent > 11)
				return;

			float timeForSearch = std::clamp(timeSpent - 0.5f, 1.f, 3.f);
			auto lastSeg = curPath->getSegment(curPath->getNumSegments() - 1);
			nextPath.reset();
			startSearch(lastSeg.getEnd(), g_Data.getLocalPlayer()->region, timeForSearch, [&](bool succeeded, Path tempPath) {
				if (!succeeded) {
					clientMessageF("%sCould not find subsequent path!", RED);

					engageDelay = -1;
					return;
				}

				clientMessageF("%sFound subsequent %s path!", tempPath.isIncomplete1() ? YELLOW : GREEN, tempPath.isIncomplete1() ? "incomplete" : "complete");

				if (tempPath.isIncomplete1()) {
					tempPath.cutoff(0.9f);
				}

				nextPath = std::make_shared<Path>(tempPath.getAllSegments(), tempPath.isIncomplete1());
				});
		}
	}
}
void FollowPathModule::onLevelRender() {
	if (!g_Data.isInGame() || g_Data.getLocalPlayer() == nullptr) {
		setEnabled(false);
		return;
	}

	if (movementController && path)
		path->draw(movementController->getCurrentPathSegment());
}