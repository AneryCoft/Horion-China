#include "AutoLeave.h"

AutoLeave::AutoLeave() : IModule(0, Category::PLAYER, "Automatically leave when the player's health falls below the threshold.") {
	registerFloatSetting("Health", &health, health, 0.f, maxHealth);
}

AutoLeave::~AutoLeave() {
}

const char* AutoLeave::getModuleName() {
	return ("AutoLeave");

}

void AutoLeave::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (!(g_Data.getRakNetInstance()->serverIp.getTextLength() < 1))
		return; //是否在服务器

	maxHealth = localPlayer->getAttribute(&HealthAttribute())->maximumValue;
	float loaclPlayerHealth = localPlayer->getAttribute(&HealthAttribute())->currentValue;

	if (loaclPlayerHealth < health) {
		CommandRequestPacket packet;
		TextHolder cmd = "/hub";
		packet.command = cmd;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
	}
}