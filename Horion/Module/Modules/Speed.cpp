#include "Speed.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Bhop", 1))
		.addEntry(EnumEntry("Lowhop", 2))
		.addEntry(EnumEntry("HiveFast", 3))
		.addEntry(EnumEntry("HiveSafe", 4));
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
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (mode.selected == 0) {
		float* speedAdr = reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
		*speedAdr = vanillaSpeed;
	}
	else if (mode.selected == 1 || mode.selected == 2) {
		speed = randomFloat(maxSpeed, minSpeed);
	}
	if (mode.selected == 3) {
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

float hiveSpeed[12] = {
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
	0.26041,
	0.24647
};

float hiveSafe[12] = {
	0.48041,
	0.45005,
	0.43040,
	0.41000,
	0.39050,
	0.38000,
	0.37000,
	0.36000,
	0.35000,
	0.34000,
	0.32041,
	0.31647
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
	else if (mode.selected == 3 || mode.selected == 4) {
		vec2_t movement = { input->forwardMovement, -input->sideMovement };
		bool pressed = movement.magnitude() > 0.f;
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		movement = { movement.x * c - movement.y * s, movement.x * s + movement.y * c };
		if (pressed && player->onGround) {
			// input->isJumping = true;
			player->jumpFromGround();
			son = true;
		}
		float safeSpeedArray;
		if (mode.selected == 3) {
			if (son)
				safeSpeedArray = hiveSpeed[index++ % 12];
			else
				safeSpeedArray = 0.24247;
		}
		else {
			if (son)
				safeSpeedArray = hiveSafe[index++ % 12];
			else
				safeSpeedArray = 0.24247;
		}
		if (player->onGround) {
			//safeSpeedArray == 0.61000;
			son = true;
			stick++;
			if (stick >= 3)
				stick = 0;
			if (g_Data.getLocalPlayer()->velocity.squaredxzlen() >= 0.200 && stick == 0) {
			}
			else if (g_Data.getLocalPlayer()->velocity.squaredxzlen() <= 0.1 && stick == 0)
				return;
		}
		if (input->isJumping) {
			moveVec.x = movement.x * safeSpeedArray;
			moveVec.y = player->velocity.y;
			moveVec.z = movement.y * safeSpeedArray;
			if (!player->onGround) {
				if (inter)
					*g_Data.getClientInstance()->minecraft->timer = 16;
				else
					*g_Data.getClientInstance()->minecraft->timer = 26;
			}
			else {
				*g_Data.getClientInstance()->minecraft->timer = 16;
				if (pressed) player->lerpMotion(moveVec);
			}
		}
		else {
			moveVec.x = movement.x * safeSpeedArray;
			moveVec.y = player->velocity.y;
			moveVec.z = movement.y * safeSpeedArray;
			if (pressed) player->lerpMotion(moveVec);
		}
		if (son && !player->onGround) {
			if (index >= 12) {
				index = 0;
				son = false;
			}
		}
		if (son && player->onGround) {
			if (index >= 5) {
				index = 0;
				//son = false;
				son = true;
			}
		}
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
			C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(moveVec.x / 1.3f, 0.f, moveVec.z / 1.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 3.13f, 0.f, player->velocity.z / 2.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
		}
	}
}
