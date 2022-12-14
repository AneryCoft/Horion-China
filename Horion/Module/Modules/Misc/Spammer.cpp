#include "Spammer.h"

Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay.") {
	registerFloatSetting("Delay", &delay, delay, 0.f, 5000.f);
	registerIntSetting("Bypass Length", &length, length, 1, 60);
	registerBoolSetting("Bypass", &bypass, bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}

void Spammer::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (!g_Data.getLocalPlayer()->checkNameTagFunc())
		return;

	if (delayTime.hasTimedElapsed(delay, true)) {
		C_TextPacket textPacket;
		textPacket.messageType = 1;
		textPacket.message.setText(bypass ? (message + " | " + Utils::randomString(length)) : message);
		textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
		textPacket.xboxUserId = g_Data.getClientInstance()->minecraftGame->xuid;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
	}
}
