#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Automatically teleports you back up if you fall down more than X blocks") {
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
		vec3_ti blockPos = vec3_ti(playerPos->x, i, playerPos->z);
		int blockId = player->region->getBlock(blockPos)->toLegacy()->blockId;
		if (blockId != 0)
			return false;
	}
	return true;
} //该函数用于判断玩家脚下是否为虚空

void AntiVoid::onTick(C_GameMode* gm) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	vec3_t* playerPos = player->getPos();

	vec3_t blockBelow = player->eyePos0;
	blockBelow.y -= player->height;
	blockBelow.y -= 0.5f;

	//	if (((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0 && ((player->region->getBlock(blockBelow)->blockLegacy))->material->isSolid) {
	//if (player->velocity.y == 0.f) {
	if (player->onGround && ((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0) {
		savePos = *playerPos;
	}

	tempPos = vec3_t(playerPos->x, savePos.y, playerPos->z);
	C_MovePlayerPacket movePacket;
	movePacket.Position = tempPos;

	if (player->fallDistance >= distance && !(onlyVoid && !isVoid())) {
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket); //防止被Mineplex的反穿墙拉回
		player->setPos(savePos);
	}
}