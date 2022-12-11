#include "Speed.h"

float speedFriction = 0.65f;
bool Faststop = true;
Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Bhop", 1))
		.addEntry(EnumEntry("Lowhop", 2))
		.addEntry(EnumEntry("Friction", 3))
		.addEntry(EnumEntry("HiveTest", 4));
	registerEnumSetting("Mode", &mode, 1);
	registerBoolSetting("FastStop", &Faststop, Faststop);
	registerFloatSetting("VanillaSpeed", &vanillaSpeed, vanillaSpeed, 0.1f, 5.f);
	registerFloatSetting("MaxSpeed", &maxSpeed, maxSpeed, 0.1f, 1.f);
	registerFloatSetting("MinSpeed", &minSpeed, minSpeed, 0.1f, 1.f);
	registerFloatSetting("Duration", &duration, duration, 0.2f, 1.5f);
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
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	// Vanilla
	if (mode.selected == 0) {
		float* speedAdr = reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
		*speedAdr = vanillaSpeed;
	}
	// Bhop && Lowhop
	else if (mode.selected == 1 || mode.selected == 2) {
		speed = randomFloat(maxSpeed, minSpeed);
	}
	// HiveOld Baddddddddddd
	/*if (mode.selected == 3) {
		std::vector<vec3_ti> sideBlocks;
		sideBlocks.reserve(8);

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);

		for (int x = -1; x <= 1; x++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && z == 0)
					continue;

				sideBlocks.push_back(vec3_ti(x, 0, z));
			}
		}

		auto pPos = *gm->player->getPos();
		pPos.y = gm->player->aabb.lower.y;
		auto pPosI = vec3_ti(pPos.floor());

		auto isObstructed = [&](int yOff, AABB* obstructingBlock = nullptr, bool ignoreYcoll = false) {
			for (const auto& current : sideBlocks) {
				vec3_ti side = pPosI.add(0, yOff, 0).add(current);
				if (side.y < 0 || side.y >= 256)
					break;
				auto block = gm->player->region->getBlock(side);
				if (block == nullptr || block->blockLegacy == nullptr)
					continue;
				C_BlockLegacy* blockLegacy = block->toLegacy();
				if (blockLegacy == nullptr)
					continue;
				AABB collisionVec;
				if (!blockLegacy->getCollisionShape(&collisionVec, block, gm->player->region, &side, gm->player))
					continue;
			}
			return false;
		};

		AABB lowerObsVec, upperObsVec;
		bool upperObstructed = isObstructed(1, &upperObsVec);

		bool lowerObstructed = isObstructed(0, &lowerObsVec);
		if (lowerObstructed || upperObstructed)
			inter = true;
		else
			inter = false;

		auto player = g_Data.getLocalPlayer();
		float yaw = player->yaw;
		if (input->spaceBarKey) {
			if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
			}
		}
	}
	*/
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

void Speed::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1)
		return;

	vec2_t movement = { input->forwardMovement, -input->sideMovement };
	bool pressed = movement.magnitude() > 0.f;
	if (!pressed && Faststop) {
		auto player = g_Data.getLocalPlayer();
		if (player != nullptr) {
			if (!(player->damageTime > 1 && false)) {
				player->velocity.x = 0.f;
				player->velocity.z = 0.f;
			}
		}
	}

	float calcYaw = (player->yaw + 90) * (PI / 180);
	float c = cos(calcYaw);
	float s = sin(calcYaw);

	vec2_t moveVec2D = { input->forwardMovement, -input->sideMovement };
	moveVec2D = { moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c };
	vec3_t moveVec;
	
	// Bhop
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
	// Lowhop
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
	// Friction
	else if (mode.selected == 3) {
		if (player->onGround && pressed)
			player->jumpFromGround();

		speedFriction *= duration;
		if (pressed) {
			if (player->onGround) {
				//if (startSpeed && !input->isJumping) player->velocity.y = 0.40;
				speedFriction = randomFloat(minSpeed, maxSpeed);
			}
			else {
				moveVec.x = moveVec2D.x * speedFriction;
				moveVec.y = player->velocity.y;
				moveVec.z = moveVec2D.y * speedFriction;
				player->lerpMotion(moveVec);
			}
		}
	}
	// HiveTest
	else if (mode.selected == 4) {
		speedFriction *= 0.9400610828399658f;
		if (pressed) {
			if (player->onGround) {
				player->jumpFromGround();
				speedFriction = randomFloat(0.5285087823867798f, 0.49729517102241516f);
			}
			else
			moveVec.x = moveVec2D.x * speedFriction;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2D.y * speedFriction;
			player->lerpMotion(moveVec);
		}
	}
}
