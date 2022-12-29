#include "InfiniteAura.h"

InfiniteAura::InfiniteAura() : IModule(0, Category::COMBAT, "Killaura but with infinite reach.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 0);
	/*packetMode = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("CubeCraft", 1));
	registerEnumSetting("Packet", &packetMode, 0);*/
	registerFloatSetting("TPDistance", &tpDistance, tpDistance, 1.f, 20.f);
	registerFloatSetting("Range", &range, range, 15.f, 128.f);
	registerIntSetting("CPS", &CPS, CPS, 1, 20);
	registerBoolSetting("RenderPos", &renderPos, renderPos);
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

		posList.clear();

		std::sort(targetList.begin(), targetList.end(), [](const C_Entity* lhs, const C_Entity* rhs) {
			vec3_t localPlayerPos = *g_Data.getLocalPlayer()->getPos();
			C_Entity* current = const_cast<C_Entity*>(lhs);
			C_Entity* other = const_cast<C_Entity*>(rhs);
			return (*current->getPos()).dist(localPlayerPos) < (*other->getPos()).dist(localPlayerPos);
			}); //距离优先

		float calcYaw = (localPlayer->yaw + 90) * (PI / 180);
		float calcPitch = (localPlayer->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.f;

		vec3_t localPlayerPos = *localPlayer->getPos();

		//bool cubecraftMode = strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.cubecraft.net") == 0;

		/*
		C_MovePlayerPacket movePlayerPacket(localPlayer, localPlayerPos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

		C_PlayerActionPacket actionPacket;
		actionPacket.action = 8; //跳跃
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

		localPlayerPos.y += 0.35f;

		movePlayerPacket = C_MovePlayerPacket(localPlayer, localPlayerPos);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

		posList.push_back(localPlayerPos);
		*/

		for (auto target : targetList) {
			vec3_t targetPos = *target->getPos();

			vec3_t tpPos = vec3_t(targetPos.x - teleportX, targetPos.y, targetPos.z - teleportZ);

			int times = ceil(localPlayerPos.dist(tpPos) / tpDistance); //需要传送的次数

			for (int n = 1; n <= times; n++) {
				vec3_t pos = localPlayerPos.add(tpPos.sub(localPlayerPos).div(times).mul(n));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(localPlayer, pos));

				posList.push_back(pos);
			}

			localPlayer->swingArm();
			g_Data.getCGameMode()->attack(target);

			//回来
			localPlayerPos = *localPlayer->getPos();
			for (int n = 1; n <= times; n++) {
				vec3_t pos = tpPos.add(localPlayerPos.sub(tpPos).div(times).mul(n));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&C_MovePlayerPacket(localPlayer, pos));
			}

			if (mode.selected == 0) {
				break;
			} //Single
		}
	}
}

void InfiniteAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!GameData::canUseMoveKeys())
		return;

	if (!posList.empty() && renderPos) {
		for (auto pos : posList) {
			DrawUtils::setColor(1, 1, 1, 0.8f);
			DrawUtils::drawBox(pos.sub(0.3f, 1.62f, 0.3f), pos.add(0.3f, 0.18f, 0.3f), 0.3f, false);
		}
	}
}

void InfiniteAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
	}

	posList.clear();
	CPSTime.resetTime();
	//计时器初始化
}
