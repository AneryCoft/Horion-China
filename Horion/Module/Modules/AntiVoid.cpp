#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Automatically teleports you back up if you fall down more than X blocks") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("Teleport", 0))
		.addEntry(EnumEntry("LagBack", 1));
	registerEnumSetting("Mode", &mode, 0);
	this->registerBoolSetting("Only Void", &this->onlyVoid, onlyVoid);
	registerIntSetting("distance", &this->distance, this->distance, 1, 20);
}

AntiVoid::~AntiVoid() {
}

const char* AntiVoid::getModuleName() {
	return ("AntiVoid");
}

bool isVoid() {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	vec3_t* playerPos = player->getPos();

	for (int i = floor(playerPos->y - 1.f); i > -65; i--) {
		vec3_ti blockPos = vec3_ti(floor(playerPos->x), i, floor(playerPos->z));
		int blockId = player->region->getBlock(blockPos)->toLegacy()->blockId;
		if (blockId != 0)
			return false;
	}
	return true;
} //该函数用于判断玩家脚下是否为虚空

void AntiVoid::onTick(C_GameMode* gm) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	lagBack = false;

	vec3_t* playerPos = player->getPos();

	vec3_t blockBelow = playerPos->add(0.f,-2.8f,0.f);

	//	if (((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0 && ((player->region->getBlock(blockBelow)->blockLegacy))->material->isSolid) {
	//if (player->velocity.y == 0.f) {
	//clientMessageF("%i", ((player->region->getBlock(blockBelow)->blockLegacy))->blockId);

	if (player->onGround && ((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0) {
		savePos = *playerPos;
	}

	C_MovePlayerPacket movePacket;
	if (player->fallDistance >= distance && !(onlyVoid && !isVoid())) {
		if (mode.selected == 0) {
			tempPos = vec3_t(playerPos->x, savePos.y, playerPos->z);
			movePacket.Position = tempPos;

			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket); //防止被Mineplex的反穿墙拉回
			player->setPos(savePos);
		}
		else if (mode.selected == 1) {
			movePacket.Position = playerPos->add(0.f, 100.f, 0.f);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket); //利用服务器的拉回
			lagBack = true;
		}
	}
}