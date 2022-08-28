#include "Speed.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Bhop", 1))
		.addEntry(EnumEntry("Lowhop", 2))
		.addEntry(EnumEntry("The Hive", 3));
	registerEnumSetting("Mode", &mode, 1);
	registerFloatSetting("VanillaSpeed", &vanillaSpeed, vanillaSpeed, 0.1f, 5.f);
	registerFloatSetting("MaxSpeed", &maxSpeed, maxSpeed, 0.1f, 1.f);
	registerFloatSetting("MinSpeed", &minSpeed, minSpeed, 0.1f, 1.f);
	registerFloatSetting("LowhopMotion", &lowhopMotion, lowhopMotion, 0.1f, 5.f);
	registerFloatSetting("Timer", &timer, timer, 20.f, 50.f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");  // 48 8D 15 ?? ?? ?? ?? 48 8B CB FF 90 ?? ?? ?? ?? 48 8B D8
}

void Speed::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (mode.selected == 0) {
		float* speedAdr = reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
		*speedAdr = vanillaSpeed;
	}
	else if (mode.selected == 1 || mode.selected == 2) {
		speed = randomFloat(maxSpeed, minSpeed);
	}

}

void Speed::onEnable() {
	if (mode.selected == 0) {
		if (g_Data.getLocalPlayer() == nullptr) {
			setEnabled(false);
			return;
		}
		else {
			origSpeed = *reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
		}
	}
}

void Speed::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	g_Data.getClientInstance()->minecraft->setTimerSpeed(20);

	if (mode.selected == 0) {
		*reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
	}
}

static float hiveSpeed[12] = {
	0.59041,
	0.58005,
	0.55040,
	0.50000,
	0.46050,
	0.42000,
	0.39000,
	0.37000,
	0.34000,
	0.30000,
	0.26000,
	0.24347
};

void Speed::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1)
		return;

	if (player->isSneaking())
		return;

	if (mode.selected == 1) {
		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (player->onGround && pressed) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
			player->jumpFromGround();
		}

		if (!moduleMgr->getModule<Fly>()->isEnabled()) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
			float calcYaw = (player->yaw + 90) * (PI / 180);
			vec3_t moveVec;
			float c = cos(calcYaw);
			float s = sin(calcYaw);
			moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };
			moveVec.x = moveVec2d.x * speed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * speed;
			if (pressed) player->lerpMotion(moveVec);
		}
	}
	else if (mode.selected == 2) {
		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (player->onGround && pressed) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
			player->velocity.y = lowhopMotion;
		}

		if (!moduleMgr->getModule<Fly>()->isEnabled()) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
			float calcYaw = (player->yaw + 90) * (PI / 180);
			vec3_t moveVec;
			float c = cos(calcYaw);
			float s = sin(calcYaw);
			moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };
			moveVec.x = moveVec2d.x * speed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * speed;
			if (pressed) player->lerpMotion(moveVec);
		}
	}
	else if (mode.selected == 3) {
		static int index = 0;
		float hivespeed;
		static bool onGround;

		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		bool pressed = moveVec2d.magnitude() > 0.f;

		if (player->onGround)
			onGround = true;

		if (onGround)
			hivespeed = hiveSpeed[index++ % 12];
		else
			hivespeed = 0.21347;

		if (player->onGround && pressed) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(20);

				player->jumpFromGround();

				float calcYaw = (player->yaw + 90) * (PI / 180);
				vec3_t moveVec;
				float c = cos(calcYaw);
				float s = sin(calcYaw);
				moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };
				moveVec.x = moveVec2d.x * hivespeed;
				moveVec.y = player->velocity.y;
				moveVec.z = moveVec2d.y * hivespeed;
				if (pressed) player->lerpMotion(moveVec);
				if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
					C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(moveVec.x / 1.3f, 0.f, moveVec.z / 1.3f)));
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
					C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 3.13f, 0.f, player->velocity.z / 2.3f)));
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
				}
			}
		if (onGround && !player->onGround) {
			if (index >= 12) {
				index = 0;
				onGround = false;
			}
		}
	}
}
