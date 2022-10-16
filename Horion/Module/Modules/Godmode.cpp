#include "Godmode.h"

Godmode::Godmode() : IModule(0, Category::MISC, "You won't die.") {
}

Godmode::~Godmode() {
}

const char* Godmode::getModuleName() {
	return ("Godmode");
}

void Godmode::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t localPlayerPos = *localPlayer->getPos();
	float posY = localPlayerPos.y + 0.8f;

		C_MovePlayerPacket movePacket;
		for (; posY >= localPlayerPos.y; posY -= 0.62f) {
			movePacket.Position = vec3_t(localPlayerPos.x + 6666, posY + 6666, localPlayerPos.z + 6666);  // send position to server
			// localPlayer->setPos(p.pos);
			if (posY <= localPlayerPos.y)
				posY = localPlayerPos.y + 0.2;
		}
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);

		if (localPlayer->onGround) {
			C_PlayerActionPacket actionPacket;
			actionPacket.action = 8;  // Send Jump Action
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
	}
}

void Godmode::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = true;
	}
}
