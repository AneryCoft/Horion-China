#include "Disabler.h"

Disabler::Disabler() : IModule(0, Category::MISC, "Makes some Anti-Cheats unworkable") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Velocity Spoof", 0))
		.addEntry(EnumEntry("CPS Cancel", 1))
		.addEntry(EnumEntry("Mineplex", 2))
		.addEntry(EnumEntry("CubeCraft", 3))
		.addEntry(EnumEntry("The Hive", 4))
		.addEntry(EnumEntry("Lifeboat", 5));
	registerEnumSetting("Mode", &mode, 0);
}

Disabler::~Disabler() {
}

const char* Disabler::getModuleName() {
	return ("Disabler");
}

void Disabler::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer == nullptr) {
		return;
	}

	vec3_t* localPlayerPos = localPlayer->getPos();

	++tick;

	if (mode.selected == 2) {
		C_MovePlayerPacket movePacket;
		movePacket.Position = *localPlayerPos;
		movePacket.pitch = localPlayer->pitch;
		movePacket.yaw = localPlayer->bodyYaw;
		movePacket.headYaw = localPlayer->yawUnused1;
		movePacket.onGround = true;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	}
	else if (mode.selected == 3) {
		//if (localPlayer->velocity.magnitude() > 0.1f) {
		C_MovePlayerPacket movePacket;
		movePacket.Position = *localPlayerPos;
		movePacket.pitch = localPlayer->pitch;
		movePacket.yaw = localPlayer->bodyYaw;
		movePacket.headYaw = localPlayer->yawUnused1;
		movePacket.onGround = false;
		for (int i = 0; i < 20; i++) {
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		}
		//}
	}
	else if (mode.selected == 4) {
		if (attackTime.hasTimedElapsed(1000.f / 10.f, true)) {
			g_Data.getCGameMode()->attack(localPlayer);
		}
	}
}

void Disabler::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (mode.selected == 0) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->velocity = vec3_t(0.1f, 0.1f, 0.1f);
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
	/*else if (mode.selected == 2) {
		static bool isUpOrDown = false;

		if (abs(g_Data.getLocalPlayer()->velocity.y) > 0.1f) {
			isUpOrDown = true;
		}

		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			if (isUpOrDown && (tick >= 40 || abs(g_Data.getLocalPlayer()->velocity.y) < 0.1f)) {
				C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->Position = movePacket->Position.add(-100.f);
				tick = 0;
				isUpOrDown = false;
			}
		}
	}*/
	else if (mode.selected == 3) {
		if (packet->isInstanceOf<NetworkLatencyPacket>()) {
			cancelSend = true;
		}
	}
	/*
	else if (mode.selected == 3 || mode.selected == 4) {
		if (packet->isInstanceOf<NetworkLatencyPacket>()) {
			NetworkLatencyPacket* networkLatencyPacket = reinterpret_cast<NetworkLatencyPacket*>(packet);
			networkLatencyPacket->timeStamp -= 10000000;
			//std::chrono::system_clock::now().time_since_epoch().count() 1s的间隔约为10000000
		}
	}
	*/
}
