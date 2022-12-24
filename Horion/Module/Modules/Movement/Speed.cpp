#include "Speed.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("BHop", 1))
		.addEntry(EnumEntry("LowHop", 2))
		.addEntry(EnumEntry("Friction", 3))
		.addEntry(EnumEntry("The Hive", 4));
	registerEnumSetting("Mode", &mode, 1);
	registerFloatSetting("VanillaSpeed", &vanillaSpeed, vanillaSpeed, 0.1f, 5.f);
	registerFloatSetting("MaxSpeed", &maxSpeed, maxSpeed, 0.1f, 1.f);
	registerFloatSetting("MinSpeed", &minSpeed, minSpeed, 0.1f, 1.f);
	registerFloatSetting("LowhopMotion", &lowhopMotion, lowhopMotion, 0.1f, 1.f);
	registerFloatSetting("FrictionDuration", &duration, duration, 0.2f, 1.5f);
	registerFloatSetting("Timer", &timer, timer, 10.f, 40.f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");
}

void Speed::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) {
		setEnabled(false);
		return;
	}

	origSpeed = *reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84);
}

void Speed::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	g_Data.getClientInstance()->minecraft->setTimerSpeed(20);

	*reinterpret_cast<float*>(g_Data.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
}

void Speed::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	//C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	// Vanilla
	if (mode.selected == 0) {
		float* speedAdr = reinterpret_cast<float*>(localPlayer->getSpeed() + 0x84);
		*speedAdr = vanillaSpeed;
	}
	// Bhop && Lowhop
	else if (mode.selected == 1 || mode.selected == 2) {
		if (maxSpeed < minSpeed)
			minSpeed = maxSpeed;

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

void Speed::onMove(C_MoveInputHandler* input) {
	auto localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->isInLava() || localPlayer->isInWater())
		return;

	static auto flyMod = moduleMgr->getModule<Fly>();
	if (flyMod->isEnabled())
		return;

	vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
	bool pressed = moveVec2d.magnitude() > 0.01f;
	float calcYaw = (localPlayer->yaw + 90) * (PI / 180);
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };

	vec3_t moveVec;

	switch (mode.selected) {
		case 1: //BunnyHop
		{
			if (localPlayer->onGround && pressed) {
				g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
				localPlayer->jumpFromGround();
			}

			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);

			moveVec.x = moveVec2d.x * speed;
			moveVec.y = localPlayer->velocity.y;
			moveVec.z = moveVec2d.y * speed;
			if (pressed)
				localPlayer->lerpMotion(moveVec);
		}
		break;
		case 2: //LowHop
		{
			if (localPlayer->onGround && pressed) {
				g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
				C_PlayerActionPacket actionPacket;
				actionPacket.action = 8; //ÌøÔ¾
				actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
				localPlayer->velocity.y = lowhopMotion;
			}

			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);

			moveVec.x = moveVec2d.x * speed;
			moveVec.y = localPlayer->velocity.y;
			moveVec.z = moveVec2d.y * speed;
			if (pressed)
				localPlayer->lerpMotion(moveVec);
		}
		break;
		case 3: //Friction Hop
		{
			speedFriction *= duration;

			if (pressed) {
				if (localPlayer->onGround) {
					localPlayer->jumpFromGround();
					g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
					speedFriction = randomFloat(minSpeed, maxSpeed);
				}
				else {
					g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);

					moveVec.x = moveVec2d.x * speedFriction;
					moveVec.y = localPlayer->velocity.y;
					moveVec.z = moveVec2d.y * speedFriction;
					localPlayer->lerpMotion(moveVec);
				}
			}
		}
		break;
		case 4: //The Hive Hop
		{
			speedFriction *= 0.9400610828399658f;

			if (pressed) {
				if (localPlayer->onGround) {
					localPlayer->jumpFromGround();
					speedFriction = randomFloat(0.5285087823867798f, 0.49729517102241516f);
				}
				else {
					moveVec.x = moveVec2d.x * speedFriction;
					moveVec.y = localPlayer->velocity.y;
					moveVec.z = moveVec2d.y * speedFriction;
					localPlayer->lerpMotion(moveVec);
				}
			}
		}
	}
}
