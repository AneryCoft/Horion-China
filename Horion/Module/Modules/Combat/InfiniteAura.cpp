#include "InfiniteAura.h"

InfiniteAura::InfiniteAura() : IModule(0, Category::COMBAT, "Killaura but with infinite reach.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("TPDistance", &tpDistance, tpDistance, 1.f, 20.f);
	registerFloatSetting("Range", &range, range, 15.f, 128.f);
	registerIntSetting("CPS", &CPS, CPS, 1, 20);
}

InfiniteAura::~InfiniteAura() {
}

const char* InfiniteAura::getModuleName() {
	return ("InfiniteAura");
}

static std::vector<C_Entity*> targetList0;

void findEntities(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto infiniteAuraMod = moduleMgr->getModule<InfiniteAura>();

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < infiniteAuraMod->range) {
		targetList0.push_back(currentEntity);
	}
}

void InfiniteAura::onTick(C_GameMode* gm) {
	//Loop through all our players and retrieve their information
	targetList0.clear();

	g_Data.forEachValidEntity(findEntities);

	if (!targetList0.empty() && CPSTime.hasTimedElapsed(1000.f / (float)CPS, true)) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

		std::sort(targetList0.begin(), targetList0.end(), [](const C_Entity* lhs, const C_Entity* rhs) {
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

		/*if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.cubecraft.net") == 0) {
			vec3_t pos = *g_Data.getLocalPlayer()->getPos();

			C_MovePlayerPacket movePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

			pos.y += 0.35f;

			movePlayerPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);
		}*/

		for (auto target : targetList0) {
			vec3_t targetPos = *target->getPos();
			vec3_t* localPlayerPos = localPlayer->getPos();
			vec3_t tpPos = vec3_t(targetPos.x - teleportX, targetPos.y, targetPos.z - teleportZ);

			float times = localPlayerPos->dist(tpPos) / tpDistance; //需要传送的次数
			for (int n = 0; n < times; n++) {
				vec3_t offs = tpPos.sub(*localPlayerPos).div(times).mul(n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(localPlayer, localPlayerPos->add(offs)));
			}

			movePacket = C_MovePlayerPacket(localPlayer, tpPos);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*target->getPos());
			movePacket.pitch = angle.x;
			movePacket.headYaw = angle.y;
			movePacket.yaw = angle.y;
			//转头

			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
			g_Data.getCGameMode()->attack(target);
			localPlayer->swingArm();

			//回来
			localPlayerPos = localPlayer->getPos();
			int backTimes = tpPos.dist(*localPlayerPos) / tpDistance;
			for (int n = 0; n < backTimes; n++) {
				vec3_t offs = tpPos.sub(*localPlayerPos).div(backTimes).mul(n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(localPlayer, localPlayerPos->add(offs)));
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
