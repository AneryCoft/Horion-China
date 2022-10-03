#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode = (*new SettingEnum(this))
		//.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Mineplex", 0))
		.addEntry(EnumEntry("CubeCraft", 1))
		.addEntry(EnumEntry("The Hive", 2))
		.addEntry(EnumEntry("OldCubeCraft", 3))
		.addEntry(EnumEntry("Nukkit", 4))
		.addEntry(EnumEntry("CancelPacket", 5));

	registerEnumSetting("Mode", &mode, 0);
}

NoFall::~NoFall() {
}

const char* NoFall::getModuleName() {
	return ("NoFall");
}

void NoFall::onSendPacket(C_Packet* packet, bool& cancelSend) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->fallDistance > 3.f) {
		if (mode.selected == 0) {
			if (packet->isInstanceOf<C_MovePlayerPacket>()) {
				C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->onGround = true;
			}
		}
		else if (mode.selected == 1) {
			if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
				PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
				authInputPacket->velocity.y = 0.f;
				authInputPacket->pos.add(0.f, 0.5f, 0.f);
				localPlayer->fallDistance = 0.f;
			}
		}
		else if (mode.selected == 5) {
			if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
				cancelSend = true;
			}
		}
	}
}

float NoFall::distanceToGround() {
	vec3_t localPlayerPos = *g_Data.getLocalPlayer()->getPos();

	for (int i = floor(localPlayerPos.y - 1.f); i > -65; i--) {
		vec3_ti blockPos = vec3_ti(floor(localPlayerPos.x), i, floor(localPlayerPos.z));
		int blockId = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;
		if (blockId != 0)
			return localPlayerPos.dist(blockPos.toVec3t()) - 1.f;
	}
}

void NoFall::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t localPlayerPos = *gm->player->getPos();

	if (localPlayer->fallDistance > 3.f) {
		switch (mode.selected) {
			/*
	case 0:{
		C_PlayerActionPacket actionPacket;
		actionPacket.action = 7; //Respawn
		actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
	}
		break;
		*/

		case 2: {
			localPlayerPos.y += 20;
			gm->player->tryTeleportTo(localPlayerPos, true, true, 1, 1);
			gm->player->onGround = true;
			gm->player->fallDistance = 0.f;
		} break;
		case 3: {
			localPlayer->velocity.y = 0.f;
			localPlayer->setPos(localPlayerPos.add(0, 0.3, 0));
		}break;
		case 4: {
			C_PlayerActionPacket actionPacket;
			actionPacket.action = 15;  //¿ªÆôÇÊ³á
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}break;
		}
	}
}
