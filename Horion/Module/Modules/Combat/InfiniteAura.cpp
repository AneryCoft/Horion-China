#include "InfiniteAura.h"

InfiniteAura::InfiniteAura() : IModule(0, Category::COMBAT, "Killaura but with infinite reach.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 0);
	packetMode = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("CubeCraft", 1));
	registerEnumSetting("Packet", &packetMode, 0);
	registerFloatSetting("TPDistance", &tpDistance, tpDistance, 1.f, 20.f);
	registerFloatSetting("Range", &range, range, 15.f, 128.f);
	registerIntSetting("CPS", &CPS, CPS, 1, 20);
}

InfiniteAura::~InfiniteAura() {
}

const char* InfiniteAura::getModuleName() {
	return ("InfiniteAura");
}

//static std::vector<C_Entity*> targetList0;

void findEntities(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto infiniteAuraMod = moduleMgr->getModule<InfiniteAura>();

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < infiniteAuraMod->range) {
		infiniteAuraMod->targetList.push_back(currentEntity);
	}
}

void InfiniteAura::onTick(C_GameMode* gm) {
	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachValidEntity(findEntities);

	if (!targetList.empty() && CPSTime.hasTimedElapsed(1000.f / (float)CPS, true)) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

		std::sort(targetList.begin(), targetList.end(), [](const C_Entity* lhs, const C_Entity* rhs) {
			vec3_t localPlayerPos = *g_Data.getLocalPlayer()->getPos();
			C_Entity* current = const_cast<C_Entity*>(lhs);
			C_Entity* other = const_cast<C_Entity*>(rhs);
			return (*current->getPos()).dist(localPlayerPos) < (*other->getPos()).dist(localPlayerPos);
			}); //距离优先

		float calcYaw = (localPlayer->yaw + 90) * (PI / 180);
		float calcPitch = (localPlayer->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.7f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.7f;

		C_MovePlayerPacket movePacket;

		vec3_t localPlayerPos = *localPlayer->getPos();

		//bool cubecraftMode = strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.cubecraft.net") == 0;
		bool cubecraftMode = mode.selected == 1;

		if (cubecraftMode) {
			C_MovePlayerPacket movePlayerPacket(localPlayer, localPlayerPos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

			C_PlayerActionPacket actionPacket;
			actionPacket.action = 8; //跳跃
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

			localPlayerPos.y += 0.35f;

			movePlayerPacket = C_MovePlayerPacket(localPlayer, localPlayerPos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);
		}

		for (auto target : targetList) {
			vec3_t targetPos = *target->getPos();
			
			vec3_t tpPos = vec3_t(targetPos.x - teleportX, targetPos.y, targetPos.z - teleportZ);

			if (!cubecraftMode) {
				float times = localPlayerPos.dist(tpPos) / tpDistance; //需要传送的次数
				for (int n = 0; n < times; n++) {
					vec3_t offs = tpPos.sub(localPlayerPos).div(times).mul(n);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(localPlayer, localPlayerPos.add(offs)));
				}
			}

			movePacket = C_MovePlayerPacket(localPlayer, tpPos);
			vec2_t angle = localPlayer->getPos()->CalcAngle(*target->getPos());
			movePacket.pitch = angle.x;
			movePacket.headYaw = angle.y;
			movePacket.yaw = angle.y;
			//转头
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);

			localPlayer->swingArm();
			g_Data.getCGameMode()->attack(target);

			//回来
			if(!cubecraftMode){
				localPlayerPos = *localPlayer->getPos();
				int backTimes = tpPos.dist(localPlayerPos) / tpDistance;
				for (int n = 0; n < backTimes; n++) {
					vec3_t offs = tpPos.sub(localPlayerPos).div(backTimes).mul(n);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(localPlayer, localPlayerPos.add(offs)));
				}
			}

			movePacket = C_MovePlayerPacket(localPlayer, *localPlayer->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);

			if (mode.selected == 0) {
				break;
			}
		}	
	}
}

void InfiniteAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
	}

	CPSTime.resetTime();
	//计时器初始化
}
