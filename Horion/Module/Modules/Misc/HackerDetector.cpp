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
		playerList.push_back(currentEntity);
}

void HackerDetector::onTick(C_GameMode* gm) {
	auto localPlayer = gm->player;

	playerList.clear();
	g_Data.forEachValidEntity(findPlayer);

	for (auto player : playerList) {
		std::string playerName = player->getNameTag()->getText();
		if (auto i = playerName.find("\n"); i != std::string::npos) {
			playerName.erase(playerName.begin() + i, playerName.end());
		} //ɾ�������е��ַ���

		float BPS = player->getBlocksPerSecond();

		bool isHurting = player->damageTime > 1; //����

		vec3_t playerPos = *player->getPos();

		bool realOnGround = localPlayer->region->getBlock(vec3_ti(playerPos.x, playerPos.y - 1.8f - 1, playerPos.z))->toLegacy()->blockId != 0;

		if (speed) {
			if (!isHurting) {
				if (abs(player->velocity.y) < 0.1f) {
					if (BPS > 5.612332f) {
						VL = BPS - 5.612332f;
						clientMessageF("%s %sSpeed-A VL:%.2f", playerName, WHITE, VL);
					}
				} //�����ˮƽ����ʱ
				else {
					if (BPS > 12.5f) {
						VL = BPS - 12.5f;
						clientMessageF("%s %sSpeed-B VL:%.2f", playerName, WHITE, VL);
					} //�������Ծʱ
				}
			}
		}

		if (fly) {
			if (!isHurting) {
				if (!realOnGround) {
					if (player->velocity.y > 0.334f) {
						VL = BPS - 0.334f;
						clientMessageF("%s %sFly-A VL:%.2f", playerName, WHITE, VL);
					} //���ϴ�ֱ����
					else if (abs(player->velocity.y) < 0.1f) {
						if (BPS > 2.f) {
							VL = BPS - 2.f;
							clientMessageF("%s %sFly-B VL:%.2f", playerName, WHITE, VL);
						}
					} //ˮƽ����
				}
			}
		}

		if (velocity) {

		}
	}
}