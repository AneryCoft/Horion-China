#include "Speed.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("Speed", 0))
		.addEntry(EnumEntry("Bhop", 1));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("Speed", &speed, 1, 0.1f, 3.f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");  // 48 8D 15 ?? ?? ?? ?? 48 8B CB FF 90 ?? ?? ?? ?? 48 8B D8
}

static int oldselected = 0;
void Speed::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (oldselected == 0 && mode.selected == 1) 
		if (g_Data.getLocalPlayer() != nullptr)
			*reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
	
	oldselected = mode.selected;
	
	if (mode.selected == 0) {
		float* speedAdr = reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
		*speedAdr = speed;
	}
}

void Speed::onEnable() {
	oldselected = mode.selected;
	if (g_Data.getLocalPlayer() == nullptr) {
		setEnabled(false);
		return;
	} else if (mode.selected == 0) {
		origSpeed = *reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
	}
}

void Speed::onDisable() {
	if (mode.selected == 0)
		if (g_Data.getLocalPlayer() != nullptr)
			*reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
}

void Speed::onMove(C_MoveInputHandler* input) {
	if (mode.selected == 1) {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		if (player->isInLava() == 1 || player->isInWater() == 1)
			return;

		if (player->isSneaking())
			return;

		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (player->onGround && pressed)
			player->jumpFromGround();
		if (!moduleMgr->getModule<Fly>()->isEnabled()) {
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

}
