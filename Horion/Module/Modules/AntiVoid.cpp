#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Automatically teleports you back up if you fall down more than X blocks") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Teleport", 0))
		.addEntry(EnumEntry("Packet", 1));
		//.addEntry(EnumEntry("The Hive", 2));
	registerEnumSetting("Mode", &mode, 0);
	registerBoolSetting("Only Void", &onlyVoid, onlyVoid);
	registerIntSetting("Distance", &distance, distance, 1, 20);
}

AntiVoid::~AntiVoid() {
}

const char* AntiVoid::getModuleName() {
	return ("AntiVoid");
}

bool isVoid() {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	vec3_t* playerPos = player->getPos();

	for (int i = floor(playerPos->y - 2.8f); i > -65; i--) {
		vec3_ti blockPos = vec3_ti(floor(playerPos->x), i, floor(playerPos->z));
		short blockId = player->region->getBlock(blockPos)->toLegacy()->blockId;
		if (blockId != 0)
			return false;
	}
	return true;
} //该函数用于判断玩家脚下是否为虚空

static std::vector<C_MovePlayerPacket> packetList;

void AntiVoid::onTick(C_GameMode* gm) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	vec3_t* playerPos = player->getPos();

	vec3_t blockBelow = playerPos->add(0.f, -2.8f, 0.f);

	//	if (((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0 && ((player->region->getBlock(blockBelow)->blockLegacy))->material->isSolid) {
	//if (player->velocity.y == 0.f) {
	//clientMessageF("%i", ((player->region->getBlock(blockBelow)->blockLegacy))->blockId);

	if (player->onGround && ((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0) {
		packetList.clear();
		savePos = *playerPos;
	}

	if (player->fallDistance >= distance && !(onlyVoid && !isVoid())) {
		if (mode.selected == 0) {
			player->setPos(savePos);
		}
		else if (mode.selected == 1) {
			for (int i = packetList.size() - 1; i > -1; i--) {
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packetList[i]);
			}
			player->setPos(savePos);
		}
		//else if (mode.selected == 2) {
		//	player->tryTeleportTo(savePos.add(20.f), true, true, 1, 1); //利用服务器的拉回
		//}
	}
}

void AntiVoid::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (g_Data.getLocalPlayer() != nullptr && !g_Data.getLocalPlayer()->onGround && mode.selected == 1) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			packetList.push_back(*movePacket);
		}
	}
}