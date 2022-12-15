#include "HackerDetector.h"

HackerDetector::HackerDetector() : IModule(0, Category::MISC, "Report cheating by other players.") {
	registerBoolSetting("Speed", &speed, speed);
	registerBoolSetting("Fly", &fly, fly);
	registerBoolSetting("Velocity", &velocity, velocity);
}

HackerDetector::~HackerDetector() {
}

const char* HackerDetector::getModuleName() {
	return ("HackerDetector");
}

static std::vector<C_Entity*> playerList;

void findPlayer(C_Entity* currentEntity, bool isRegularEntitie) {
	if (Target::isValidTarget(currentEntity))
		if (currentEntity->isPlayer())
			playerList.push_back(currentEntity);
}

void HackerDetector::onTick(C_GameMode* gm) {
	auto localPlayer = gm->player;

	playerList.clear();

	if (localPlayer == nullptr)
		return;

	g_Data.forEachValidEntity(findPlayer);

	for (auto player : playerList) {
		std::string playerName = player->playerName.getText();
		//playerName = Utils::sanitize(playerName); //编码限制

		float BPS = player->getBlocksPerSecond();

		bool isHurting = player->damageTime > 1; //击退

		vec3_t playerPos = *player->getPos();

		bool realOnGround = localPlayer->region->getBlock(vec3_ti(playerPos.x, playerPos.y - 1.8f - 1, playerPos.z))->toLegacy()->blockId != 0;

		if (speed) {
			if (!isHurting) {
				if (abs(player->velocity.y) < 0.1f) {
					if (BPS > 12.f) { //5.612332f
						VL = BPS - 12.f;
						if (VL < 100.f) {
							clientMessageF(u8"%s %sSpeed-A VL:%.2f", playerName.c_str(), WHITE, VL);
						}
					}
				} //当玩家水平行走时
				else {
					if (BPS > 20.f) { //12.5f
						VL = BPS - 20.f;
						clientMessageF(u8"%s %sSpeed-B VL:%.2f", playerName.c_str(), WHITE, VL);
					} //当玩家跳跃时
				}
			}
		}

		if (fly) {
			if (!isHurting) {
				if (!realOnGround) {
					if (player->velocity.y > 0.5f) { //0.334f
						VL = player->velocity.y - 0.5f;
						clientMessageF(u8"%s %sFly-A VL:%.2f", playerName.c_str(), WHITE, VL);
					} //向上垂直飞行
					else if (abs(player->velocity.y) < 0.1f) {
						if (BPS > 13.f) {
							VL = BPS - 13.f;
							clientMessageF(u8"%s %sFly-B VL:%.2f", playerName.c_str(), WHITE, VL);
						}
					} //水平飞行
				}
			}
		}

		if (velocity) {
			if (player->damageTime > 1 && player->damageTime < 10) {
				if (abs(player->velocity.y) < 0.1f)
					clientMessageF(u8"%s %sVelocity", playerName.c_str(), WHITE);
			}
		}
	}
}