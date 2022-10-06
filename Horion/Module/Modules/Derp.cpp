#include "Derp.h"

Derp::Derp() : IModule(0, Category::MISC, "Rotation!") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Stroke", 0))
		.addEntry(EnumEntry("Scroll", 1))
		.addEntry(EnumEntry("Gyro", 3))
		.addEntry(EnumEntry("Down", 4));
	registerEnumSetting("Mode", &mode, 0);
	registerBoolSetting("packet mode", &packetMode, packetMode);
}

Derp::~Derp() {
}

const char* Derp::getModuleName() {
	return "Derp";
}

void Derp::onPlayerTick(C_Player* player) {
	switch (mode.selected) {
	case 0:
	{
		if (packetMode) {
			C_MovePlayerPacket packet(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			packet.pitch = (float)(rand() % 360);
			packet.yaw = (float)(rand() % 360);
			packet.headYaw = (float)(rand() % 360);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
		}
		else {
			player->pitch = (float)(rand() % 360);
			player->bodyYaw = (float)(rand() % 360);
		}

		if (counter < 360)
			counter++;
		else
			counter = 0;
	}
	break;
	case 1:
	{
		if (packetMode) {
			C_MovePlayerPacket packet(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			packet.pitch = (float)(counter % 360);
			packet.yaw = (float)(counter % 360);
			packet.headYaw = (float)(counter % 360);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
		}
		else {
			player->pitch = (float)(counter % 360);
			player->bodyYaw = (float)(counter % 360);
		}

		if (counter < 360)
			counter++;
		else
			counter = 0;
	}
	break;
	case 2:
	{
		if (packetMode) {
			C_MovePlayerPacket packet(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			packet.pitch = 60.f;
			packet.yaw = (float)(counter);
			packet.headYaw = (float)(counter);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
		}
		else {
			player->pitch = 60.f;
			player->bodyYaw = (float)(counter);
			player->yawUnused1 = (float)(counter);
		}

		if (counter < 180)
			counter++;
		else
			counter = -180;
	}
	break;
	case 3:
	{
		if (packetMode) {
			C_MovePlayerPacket packet(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			packet.pitch = 89.9f;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
		}
		else {
			player->pitch = 89.9f;
		}
	}
	}
}
