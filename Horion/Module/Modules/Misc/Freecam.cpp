#include "Freecam.h"

Freecam::Freecam() : IModule('V', Category::MISC, "Move your camera without moving the player.") {
	registerFloatSetting("Speed", &speed, speed, 0.50f, 1.25f);
}

Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

void Freecam::onEnable() {
	auto localPlayer = g_Data.getLocalPlayer();
	if (localPlayer != nullptr) {
		oldPos = *localPlayer->getPos();
		oldOffset = localPlayer->aabb.upper.sub(localPlayer->aabb.lower);

		pos = oldPos;
		pitch = localPlayer->pitch;
		yaw = localPlayer->yaw;
	}
}

void Freecam::onDisable() {
	auto plr = g_Data.getLocalPlayer();
	if (plr) {
		plr->setPos(oldPos);
		//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		plr->aabb.upper = plr->aabb.lower.add(oldOffset);
	}
}

void Freecam::onTick(C_GameMode* gm) {
	gm->player->fallDistance = 0.f;
	gm->player->velocity = vec3_t(0, 0, 0);
	gm->player->aabb.upper.y = gm->player->aabb.lower.y - 1.8f;
}

void Freecam::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
	bool pressed = moveVec2d.magnitude() > 0.01f;

	float calcYaw = (player->yaw + 90) * (PI / 180);
	vec3_t moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };
	moveVec.x = moveVec2d.x * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = moveVec2d.y * speed;
	if (pressed)
		player->lerpMotion(moveVec);
	//C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
	if (input->isJumping) {
		player->velocity.y += 0.50f;
	}
	if (input->isSneakDown) {
		player->velocity.y -= 0.50f;
	}
}

void Freecam::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (packet->isInstanceOf<C_PlayerActionPacket>()) {
		C_PlayerActionPacket* packets = reinterpret_cast<C_PlayerActionPacket*>(packet);
		if (packets->action == 9) { //²»·¢ËÍ¼²ÅÜ°ü
			cancelSend = true;
		}
	}
	else if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		cancelSend = true;
	}
	else if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		PlayerAuthInputPacket* packets = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		packets->pos = pos;
		packets->pitch = pitch;
		packets->yaw = yaw;
		packets->velocity = velocity;
	}
}