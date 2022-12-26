#include "Fly.h"

Fly::Fly() : IModule('F', Category::MOVEMENT, "Fly to the sky") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Creative", 0))
		.addEntry(EnumEntry("AirStuck", 1))
		.addEntry(EnumEntry("Jetpack", 2))
		.addEntry(EnumEntry("Jetpack2", 3))
		.addEntry(EnumEntry("Motion", 4))
		.addEntry(EnumEntry("NoClip", 5))
		.addEntry(EnumEntry("AirWalk", 6))
		.addEntry(EnumEntry("Jump", 7))
		//.addEntry(EnumEntry("The Hive", 7));
		.addEntry(EnumEntry("Old CubeCraft", 8));
		//.addEntry(EnumEntry("CubeCraft", 9));
	registerEnumSetting("Mode", &mode, 4);
	registerFloatSetting("Horizontal Speed", &horizontalSpeed, horizontalSpeed, 0.1f, 10.f);
	registerFloatSetting("Vertical Speed", &verticalSpeed, verticalSpeed, 0.1f, 10.f);
	registerFloatSetting("Timer", &timer, timer, 20.f, 100.f);
	registerBoolSetting("Ground Spoof", &groundSpoof, groundSpoof);
	registerBoolSetting("Elytra Spoof", &elytraSpoof, elytraSpoof);
	//registerBoolSetting("Damage", &damage, damage);
}

Fly::~Fly() {
}

const char* Fly::getModuleName() {
	return ("Fly");
}

void Fly::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	gameTick = 0;

	/*if (damage) {
		C_MovePlayerPacket packet;
		packet.onGround = false;
		packet.Position = g_Data.getLocalPlayer()->getPos()->add(0.f, 4.f, 0.f);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
	}*/

	switch (mode.selected) {
	case 7:
		y = g_Data.getLocalPlayer()->aabb.lower.y;
		break;
	case 8:
		g_Data.getLocalPlayer()->setPos((*g_Data.getLocalPlayer()->getPos()).add(vec3_t(0, 1, 0)));
		break;
	}

	if (elytraSpoof) {
		C_PlayerActionPacket actionPacket;
		actionPacket.action = 15;  //开启鞘翅
		actionPacket.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
	}
}

void Fly::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);

	switch (mode.selected) {
	case 0:
		if (g_Data.getLocalPlayer()->gamemode != 1)
			g_Data.getLocalPlayer()->canFly = false;
		break;
	case 5:
		g_Data.getLocalPlayer()->aabb.upper.y = g_Data.getLocalPlayer()->aabb.lower.y + 1.8f;
	case 4:
	case 8:
	case 9:
		g_Data.getLocalPlayer()->velocity = vec3_t(0.f, 0.f, 0.f);
		break;
	}
}

void Fly::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	++gameTick;

	switch (mode.selected) {
	case 0:
		gm->player->canFly = true;
		break;
	case 2: {
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * horizontalSpeed;
		moveVec.y = sin(calcPitch) * horizontalSpeed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * horizontalSpeed;

		gm->player->lerpMotion(moveVec);
	} break;
	case 3: {
		if (gameTick >= 5) {
			float calcYaw = (gm->player->yaw + 90) * (PI / 180);
			float calcPitch = (gm->player->pitch) * -(PI / 180);

			vec3_t pos = *g_Data.getLocalPlayer()->getPos();
			C_MovePlayerPacket a(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			pos.y += 0.35f;
			a = C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);

			gm->player->velocity.y = 0.465f;
			vec3_t moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * horizontalSpeed;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * horizontalSpeed;

			gm->player->velocity.x = moveVec.x;
			gm->player->velocity.z = moveVec.z;

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

			pos = *gm->player->getPos();
			g_Data.getLocalPlayer()->setPos(vec3_t(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

			gm->player->velocity.y -= 0.15f;
			gameTick = 0;
		}
	} break;
	case 5:
		gm->player->aabb.upper.y = gm->player->aabb.lower.y - 1.8f;
	//case 9:
	case 4:
		gm->player->velocity = vec3_t(0.f, 0.f, 0.f);
		break;
	case 6:
		gm->player->onGround = true;
		gm->player->velocity.y = 0.f;
		break;
	case 8: {
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);

		vec3_t pos = *g_Data.getLocalPlayer()->getPos();
		pos.y += 1.3f;
		C_MovePlayerPacket a(g_Data.getLocalPlayer(), pos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		pos.y -= 1.3f;
		C_MovePlayerPacket a2(g_Data.getLocalPlayer(), pos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a2);

		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * horizontalSpeed;
		moveVec.z = sin(calcYaw) * horizontalSpeed;

		gm->player->lerpMotion(moveVec);

		if (gameTick >= 5) {
			gameTick = 0;
			float yaw = gm->player->yaw * (PI / 180);
			float length = 4.f;

			float x = -sin(yaw) * length;
			float z = cos(yaw) * length;

			gm->player->setPos(pos.add(vec3_t(x, 0.5f, z)));
		}
	} break;
	}
}

void Fly::onMove(C_MoveInputHandler* input) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	vec3_t localPlayerPos = *localPlayer->getPos();

	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);

	switch (mode.selected) {
	case 4:
	case 5:
	{
		float yaw = localPlayer->yaw;
		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (input->isJumping) {
			localPlayer->velocity.y += verticalSpeed;
			localPlayer->fallDistance = 0.f;
		}
		if (input->isSneakDown) {
			localPlayer->velocity.y -= verticalSpeed;
			localPlayer->fallDistance = 0.f;
		}
		if (input->right) {
			yaw += 90.f;
			if (input->forward)
				yaw -= 45.f;
			else if (input->backward)
				yaw += 45.f;
		}
		if (input->left) {
			yaw -= 90.f;
			if (input->forward)
				yaw += 45.f;
			else if (input->backward)
				yaw -= 45.f;
		}
		if (input->backward && !input->left && !input->right)
			yaw += 180.f;

		if (pressed) {
			float calcYaw = (yaw + 90.f) * (PI / 180.f);
			vec3_t moveVec;
			moveVec.x = cos(calcYaw) * horizontalSpeed;
			moveVec.y = localPlayer->velocity.y;
			moveVec.z = sin(calcYaw) * horizontalSpeed;
			localPlayer->lerpMotion(moveVec);
		}
	} break;
	case 7:
		if (localPlayer->aabb.lower.y < y) {
			localPlayer->jumpFromGround();
		}
		break;

		/*case 7:
			hiveVelocity++;
			enabledTick++;

			float safeSpeedArray = hiveSpeedArray[hiveSpeedIndex++ % 15];

			float yaw = localPlayer->yaw;
			float calcYaw = (yaw + 90.f) * (PI / 180.f);
			vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
			bool pressed = moveVec2d.magnitude() > 0.01f;
			vec3_t moveVec;

			if (pressed) {
				moveVec.x = cos(calcYaw) * safeSpeedArray;
				moveVec.z = sin(calcYaw) * safeSpeedArray;

				if (hiveVelocity >= 3) {
					hiveVelocity = 0; moveVec.y = -0.08f;
				}

				if (enabledTick >= 5 && pressed && !localPlayer->onGround)
					localPlayer->lerpMotion(moveVec);
			}
			else {
				localPlayer->velocity.x = 0.f;
				localPlayer->velocity.z = 0.f;
			}*/

	/*case 9:
	{
		vec3_t moveVec;
		float calcYaw = (localPlayer->yaw + 90) * (PI / 180);
		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };

		if (localPlayer->onGround)
			localPlayer->jumpFromGround();

		if (input->isJumping) {
			moveVec.y = localPlayer->velocity.y + verticalSpeed;
		}
		if (input->isSneakDown) {
			moveVec.y = localPlayer->velocity.y - verticalSpeed;
		}

		if (gameTick >= 3) {
			moveVec.x = moveVec2d.x * horizontalSpeed;
			moveVec.z = moveVec2d.y * horizontalSpeed;
			gameTick = 0;
		}
		else {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(100.f);
			moveVec.x = moveVec2d.x * 0.1f;
			moveVec.y = 0.f;
			moveVec.z = moveVec2d.y * 0.1f;
		}

		localPlayer->lerpMotion(moveVec);
	}break;*/
	}
}

void Fly::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (packet->isInstanceOf<C_PlayerActionPacket>()) {
		C_PlayerActionPacket* packets = reinterpret_cast<C_PlayerActionPacket*>(packet);
		if (elytraSpoof) {
			if (packets->action == 16) { //客户端自动发送的关闭鞘翅包
				cancelSend = true; //取消发送这个包
			}
		}
	}else if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* packets = reinterpret_cast<C_MovePlayerPacket*>(packet);
		if (groundSpoof) {
			packets->onGround = true;
		}
	}
}
