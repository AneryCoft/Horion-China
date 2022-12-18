#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit becomes a critical hit.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("MovePacket", 0))
		.addEntry(EnumEntry("AuthInputPacket", 1))
		.addEntry(EnumEntry("LowHop", 2))
		.addEntry(EnumEntry("Jump", 3));
	registerEnumSetting("Mode", &mode, 0);
	registerIntSetting("HurtTime", &hurttime, hurttime, 0, 10);
}

Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

void Criticals::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (mode.selected == 1) {
		static int tick = 0;
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			switch (tick) {
			case 0:{
				C_PlayerActionPacket actionPacket;
				actionPacket.action = 8;
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

				authPacket->pos.y += 1.25f;
			}
				break;
			case 1:
				authPacket->pos.y += 1.24f;
				break;
			case 2:
				authPacket->pos.y += 1.23f;
				break;
			case 3:
				authPacket->pos.y += 1.22f;
				break;
			case 4:
				authPacket->pos.y += 1.21f;
				break;
			case 5:
				authPacket->pos.y += 1.20f;
				break;
			case 6:
				authPacket->pos.y += 1.19f;
				break;
			case 7:
				authPacket->pos.y += 1.18f;
				break;
			case 8:
				authPacket->pos.y += 1.17f;
				break;
			case 9:
				authPacket->pos.y += 1.16f;
				tick = 0;
				return;
			}

			++tick;
		}
	}
	/*
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = false;
	}
	*/
}

void Criticals::onAttack(C_Entity* attackEnt) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	}
	//使onAttack函数不再执行两次


	if (attackEnt->damageTime > hurttime)
		return;

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t* localPlayerPos = localPlayer->getPos();

	switch (mode.selected) {
	case 0:
	{
		C_MovePlayerPacket movePacket;
		movePacket.onGround = false;
		movePacket.Position = vec3_t(localPlayerPos->x, localPlayerPos->y + randomFloat(0.09f, 0.3f), localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		movePacket.Position = vec3_t(localPlayerPos->x, localPlayerPos->y + randomFloat(0.001f, 0.09f), localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	}
	break;
	case 1:
	{
		/*C_PlayerActionPacket actionPacket;
		actionPacket.action = 8;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		PlayerAuthInputPacket authInputPacket;
		authInputPacket.pos = vec3_t(localPlayerPos->x, localPlayerPos->y + 1.25f, localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&authInputPacket);
		authInputPacket.pos = vec3_t(localPlayerPos->x, localPlayerPos->y + 1.1f, localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&authInputPacket);
		authInputPacket.pos = vec3_t(localPlayerPos->x, localPlayerPos->y + 0.8f, localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&authInputPacket);
		authInputPacket.pos = vec3_t(localPlayerPos->x, localPlayerPos->y + 0.5f, localPlayerPos->z);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&authInputPacket);*/
	}
	break;
	case 2:
	{
		localPlayer->velocity.y = randomFloat(0.1f, 0.3f);
		localPlayer->onGround = false;
	}
	break;
	case 3:
	{
		if (localPlayer->onGround)
			localPlayer->velocity.y = 0.42f;
	}
	break;
	case 4:
	{
		/*C_Player* entity = reinterpret_cast<C_Player*>(attackEnt);
		entity->_crit(localPlayer);*/ //_crit只能给自己生成暴击粒子
	}
	break;
	}
}
