#include "Godmode.h"

Godmode::Godmode() : IModule(0, Category::MISC, "You won't die on EaseCation.") {
}

Godmode::~Godmode() {
}

const char* Godmode::getModuleName() {
	return ("Godmode");
}

void Godmode::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(g_Data.getLocalPlayer(), g_Data.getLocalPlayer()->getPos()->add(66666)));
}
