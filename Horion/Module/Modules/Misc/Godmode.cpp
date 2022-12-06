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
	if (localPlayer == nullptr)
		return;

	C_MovePlayerPacket movePacket(localPlayer, localPlayer->getPos()->add(99999.f));
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
}
