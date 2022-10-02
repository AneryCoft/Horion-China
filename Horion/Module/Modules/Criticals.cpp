#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit becomes a critical hit.") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("MovePacket", 0))
		.addEntry(EnumEntry("AuthInputPacket", 1))
		.addEntry(EnumEntry("LowHop", 2));
	registerEnumSetting("Mode", &mode, 0);
}

Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

void Criticals::onSendPacket(C_Packet* packet, bool& cancelSend) {
	/*
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = false;
	}
	*/
}

void Criticals::onAttack(C_Entity*) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	} //使onAttack函数不再执行两次

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t* localPlayerPos = localPlayer->getPos();

	switch (mode.selected) {
	case 0:
	{
		C_MovePlayerPacket movePacket;
		movePacket.onGround = false;
		movePacket.Position = vec3_t(localPlayerPos->x, localPlayerPos->y + randomFloat(0.09f, 0.3f), localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		movePacket.Position = vec3_t(localPlayerPos->x, localPlayerPos->y + randomFloat(0.001f, 0.09f), localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	}
	break;
	case 1:
	{
		PlayerAuthInputPacket authInputPacket;
		authInputPacket.pos = vec3_t(localPlayerPos->x, localPlayerPos->y + randomFloat(0.1f, 0.5f), localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&authInputPacket);
	}
	break;
	case 2:
	{
		localPlayer->velocity.y = randomFloat(0.1f, 0.3f);
		localPlayer->onGround = false;
	}
	break;
	}
}
