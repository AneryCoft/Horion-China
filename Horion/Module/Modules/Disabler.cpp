#include "Disabler.h"

Disabler::Disabler() : IModule(0, Category::MISC, "Makes some Anti-Cheats unworkable") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Velocity Spoof", 0))
		.addEntry(EnumEntry("CPS Cancel", 1))
		.addEntry(EnumEntry("Mineplex", 2))
		.addEntry(EnumEntry("CubeCraft", 3))
		.addEntry(EnumEntry("The Hive", 4));
	registerEnumSetting("Mode", &mode, 0);
}

Disabler::~Disabler() {
}

const char* Disabler::getModuleName() {
	return ("Disabler");
}

void Disabler::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t* localPlayerPos = localPlayer->getPos();

	if (localPlayer == nullptr) {
		return;
	}

	++tick;

	if (mode.selected == 2) {
		C_MovePlayerPacket movePacket;

		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	}
	else if (mode.selected == 3) {
		if (localPlayer->velocity.magnitude() > 0.1f) {
			C_MovePlayerPacket movePacket(localPlayer, *localPlayerPos);
			movePacket.onGround = false;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		}
	}
	else if (mode.selected == 4) {
		if(attackTime.hasTimedElapsed(1000.f / 7.f, true)) {
			g_Data.getCGameMode()->attack(localPlayer);
		}
	}
}

void Disabler::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (mode.selected == 0) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->velocity = vec2_t(0.f, 0.f);
		}
	}
	else if (mode.selected == 1) {
		if (packet->isInstanceOf<LevelSoundEventPacket>()) {
			LevelSoundEventPacket* soundEventPacket = reinterpret_cast<LevelSoundEventPacket*>(packet);
			if (soundEventPacket->sound == 43 || soundEventPacket->sound == 42) //sound 42是空挥手时的数值 也会被计算进CPS 但是攻击的时候不发那个包 
				//soundEventPacket->sound = 0; 
				cancelSend = true;
		} //绕过EaseCation服务器CPS检测 
	}
}
