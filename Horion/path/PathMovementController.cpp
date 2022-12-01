#include "PathMovementController.h"

#include "../../Utils/Logger.h"
#include <utility>
#include "../Module/ModuleManager.h"
PathMovementController::PathMovementController(std::shared_ptr<Path> path) : currentPath(path) {
}

void PathMovementController::step(C_LocalPlayer* player, C_MoveInputHandler* movementHandler) {
	movementHandler->clearMovementState();
	if (this->stateInfo.currentPathSegment < 0 || this->stateInfo.currentPathSegment >= this->currentPath->getNumSegments()) {
		this->overrideViewAngles = false;
		return;
	}

	if (!this->currentPath->isInitialized1()) {
		this->currentPath->initPathSegments();
	}

	auto pPos = player->eyePos0;
	pPos.y -= 1.62f;
	vec3_ti playerNode((int)floorf(pPos.x), (int)roundf(pPos.y), (int)floorf(pPos.z));

	auto curSeg = this->currentPath->getSegment(this->stateInfo.currentPathSegment);

	if (!curSeg.isInValidPosition(playerNode)) {
		logF("invalid position %i %i %i, The Invalid Pos: %i %i %i", curSeg.getSegmentType(), this->stateInfo.currentPathSegment, this->stateInfo.currentPathSegment > 0 ? this->currentPath->getSegment(this->stateInfo.currentPathSegment - 1).getSegmentType() : 0, playerNode.x, playerNode.y, playerNode.z);
		this->stateInfo.recoverToStartPos = true;
	}

	auto startBpos = curSeg.getStart();
	auto start = startBpos.toVec3t().add(0.5f, 0, 0.5f);
	auto endBpos = curSeg.getEnd();
	auto end = endBpos.toVec3t().add(0.5f, 0, 0.5f);

	auto nextSegEnd = vec3_t();

	bool hasNextSeg = this->stateInfo.currentPathSegment < this->currentPath->getNumSegments() - 1;
	if (hasNextSeg)
		nextSegEnd = this->currentPath->getSegment(this->stateInfo.currentPathSegment + 1).getEnd().toVec3t().add(0.5f, 0, 0.5f);

	auto walkTarget = end;
	bool enableNextSegmentSmoothing = true;
	float dComp = 0.5f;
	vec3_t addedDiff{ 0, 0, 0 };

	// we should probably make seperate classes for each segment type at some point, but im just doing it here for now for faster prototyping
	switch (curSeg.getSegmentType()) {
	case JUMP: {
		if (player->onGround) {
			if (fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f) {  // Check for end condition
				this->stateInfo.nextSegment();
				break;
			}
			auto tangent = end.sub(start);
			tangent.y = 0;
			tangent = tangent.normalize();
			auto crossTangent = tangent.cross({ 0, 1, 0 });

			if ((player->getTicksUsingItem() > 0 || fabsf(player->velocity.dot(crossTangent)) > 0.02f) && fabsf(pPos.y - end.y) > 0.1f) {
				walkTarget = start;
				goto WALK;
			}

			if (pPos.y - end.y > -0.01f) {
				goto WALK;
			}

			auto lastPossibleJumpTarget = start.add(tangent.mul(0.25f));
			walkTarget = start.add(tangent); // This is not actually on a block anymore, but if we make this smaller the movement controller will stop moving at the jump target

			auto posToJumpTarg = lastPossibleJumpTarget.sub(pPos).dot(tangent);

			float maxJumpDist = 0.5f;

			if (posToJumpTarg < maxJumpDist && posToJumpTarg > 0) {
				// jump
				movementHandler->isJumping = 1;
				goto WALK;
			}
		}
		goto WALK;
	} break;
	case DROP: {
		bool inWater = player->isInWater();
		 if (player->onGround || inWater) {
			dComp = 1;
			if (fabsf(pPos.y - end.y) < (inWater ? 0.2f : 0.1f) && pPos.sub(end).magnitudexz() < 0.5f && player->velocity.y > -0.1f) {// Check for end condition
				this->stateInfo.nextSegment();
				break;
			}
			else if (inWater) {
				if (pPos.y < end.y || player->velocity.y < 0.12f)
					movementHandler->isJumping = 1;
			}
		}
		else {
			dComp = 3;
		}
		 if (start.sub(end).magnitudexz() < 1.1f) {
			// hug the wall on drop
			auto tangent = end.sub(start);
			tangent.y = 0;
			walkTarget = start.add(tangent.mul(0.5f + player->width * 0.5f + 0.1f));
			walkTarget.y = end.y;
		}

		goto WALK;
	} break;
	case PARKOUR_JUMP_SINGLE: {
		if (player->onGround) {
			if (fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f) {// Check for end condition
				this->stateInfo.nextSegment();
				break;
			}
			if (player->getTicksUsingItem() > 0 && fabsf(pPos.y - start.y) < 0.1f && pPos.dist(end) > 0.85f) {
				walkTarget = start;
				goto WALK;
			}
			auto tangent = end.sub(start);
			tangent.y = 0;
			tangent = tangent.normalize();

			float distToEnd = end.sub(pPos).dot(tangent);
			if (distToEnd > 0.f && distToEnd < 0.5f + 0.35f) {
				// maybe stuck on the block above end pos?
				walkTarget = end;
				goto WALK;
			}

			auto lastPossibleJumpTarget = start.add(tangent.mul(0.5f + 0.3f));
			walkTarget = start.add(tangent); // This is not actually on a block anymore, but if we make this smaller the movement controller will stop moving at the jump target

			auto posToJumpTarg = lastPossibleJumpTarget.sub(pPos).dot(tangent);

			float maxJumpDist = 0.3f;
			if (start.sub(end).magnitudexz() <= 1.1f)
				maxJumpDist = 0.7f;
			if (posToJumpTarg < maxJumpDist && posToJumpTarg > 0 && player->velocity.dot(tangent) > 0.07f) {
				// jump
				player->setSprinting(true);
				movementHandler->isJumping = 1;
				player->setSprinting(true);
				goto WALK;
			}
			goto WALK;
		}
		else {
			enableNextSegmentSmoothing = false;
			auto tangent = end.sub(start);
			tangent.y = 0;
			tangent = tangent.normalize();
			walkTarget = end.sub(tangent.mul(0.4f));
			dComp = 2;
			goto WALK;
		}
	} break;
	case WATER_WALK: {
		{
			player->setSprinting(curSeg.isAllowingSprint());
			auto tangent = end.sub(start);
			bool isVertical = tangent.magnitudexz() < 0.1f && fabsf(tangent.y) > 0.5f;

			if (isVertical) {
				player->setSprinting(curSeg.isAllowingSprint());
				if (pPos.sub(end).magnitudexz() < 0.3f && fabsf(pPos.y - end.y) < 0.35f) {
					this->stateInfo.nextSegment();
					break;
				}

				if (pPos.y + 0.1f < end.y)
					movementHandler->isJumping = 1;
				else if (pPos.y > end.y)
					movementHandler->isSneakDown = 1;

			}
			else {
				static auto jees = moduleMgr->getModule<Jesus>();
				player->setSprinting(curSeg.isAllowingSprint());
				// if player is in the water
				if (player->isInWater() || player->isOverWater()) {
					player->setSprinting(curSeg.isAllowingSprint());
					if (!jees->isEnabled()) {
						jees->setEnabled(true);
					}
				}
				else {
					player->setSprinting(curSeg.isAllowingSprint());
					if (jees->isEnabled()) {
						player->setPos((*player->getPos()).add(0.f, 0.01f, 0.f));
					}
				}
				if (!player->isOverWater()) {
					jees->setEnabled(false);
				}
				player->setSprinting(curSeg.isAllowingSprint());
				tangent.y = 0;
				tangent = tangent.normalize();
				auto crossTangent = tangent.cross({ 0, 1, 0 });
				float sideError = fabsf(pPos.sub(end).dot(crossTangent));
				if (sideError < 0.2f /*make sure we're not drifting to the side to much*/ && fabsf(pPos.sub(end).dot(tangent)) < 0.4f) {
					this->stateInfo.nextSegment();
					break;
				}
				if (end.y > start.y && sideError > 0.15f && pPos.y - end.y < -0.1f)
					walkTarget = start.add(tangent.mul(0.2f));  // center if we need to get up a block
			}

			vec3_t flow{};

			auto block = player->region->getBlock(playerNode);
			if (!block->toLegacy()->material->isLiquid) {
				player->setSprinting(curSeg.isAllowingSprint());
				auto mod = playerNode.add(0, -0.9, 0);
				block = player->region->getBlock(mod);

				if (block->toLegacy()->material->isLiquid)
					player->setSprinting(curSeg.isAllowingSprint());
				block->toLegacy()->liquidGetFlow(&flow, player->region, &mod);
			}
			else {
				player->setSprinting(curSeg.isAllowingSprint());
				block->toLegacy()->liquidGetFlow(&flow, player->region, &playerNode);
			}

			flow = flow.mul(-1 * 0.07f * 10);
			addedDiff = flow;
		}
		goto WALK;
	} break;
	WALK:;
	case WALK: {
		player->setSprinting(curSeg.isAllowingSprint());

		auto pPosD = pPos; // p

		if (!player->onGround && dComp < 2) {
			dComp = 2;
		}

		pPosD = pPosD.add(player->velocity.mul(dComp, 0, dComp));  // d

		if (player->onGround && end.y < start.y && fabsf(start.y - pPosD.y) < 0.1f && player->getTicksUsingItem() > 0 && end.sub(start).magnitudexz() > 1.5f) {
			// drop with a gap
			// player is using item, walk back to start pos
			walkTarget = start;
		}

		vec3_t diff3d = walkTarget.sub(pPosD);
		vec2_t diff2d = { diff3d.x, diff3d.z };
		float diffMag = diff2d.magnitude();
		if (enableNextSegmentSmoothing && hasNextSeg && diffMag < 0.2f && fabsf(end.y - pPosD.y) < (player->isInWater() ? 0.5f : 0.1f)) { // Start taking the next segment into account when we're very close to our destination
			auto tangent = nextSegEnd.sub(end).normalize();
			diff3d = end.add(tangent.mul(0.3f)).sub(pPosD);
			diff2d = { diff3d.x, diff3d.z };
			diffMag = diff2d.magnitude();
		}
		diff2d = diff2d.div(fmaxf(0.15f, diffMag));

		addedDiff.y = 0;
		if (!addedDiff.iszero()) { // water flow
			diff2d = diff2d.add(addedDiff.x, addedDiff.z);
			diff2d = diff2d.div(fmaxf(1, diff2d.magnitude()));
		}

		float yaw = player->yaw;
		auto forward = vec2_t::fromAngle(yaw * RAD_DEG);
		auto right = forward.cross();

		movementHandler->forwardMovement = forward.dot(diff2d);
		movementHandler->sideMovement = -right.dot(diff2d);

		//logF("%.2f %.2f %.2f %i", diff2d.x, diff2d.y, pPos.y, player->onGround);

		if (pPos.dist(end) < 0.2f) {
			if (hasNextSeg || player->velocity.magnitudexz() < 0.02f /*slow down for last segment*/) {
				this->stateInfo.nextSegment();
				break;
			}
		}

	} break;
	default:
		__debugbreak();
	}
}
int PathMovementController::getCurrentPathSegment() const {
	return stateInfo.currentPathSegment;
}
const std::shared_ptr<Path>& PathMovementController::getCurrentPath() const {
	return currentPath;
}