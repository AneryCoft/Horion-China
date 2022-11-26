#include "BowAimbot.h"

BowAimbot::BowAimbot() : IModule(0, Category::COMBAT, "Aimbot, but for bows.") {
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Angle", 1));
	registerEnumSetting("Priority", &priority, 0);
	registerFloatSetting("Rotation FOV", &FOV, FOV, 10.f, 180.f);
	registerBoolSetting("Silent", &silent, silent);
	registerBoolSetting("Predict", &predict, predict);
	registerBoolSetting("ThroughBlock", &throughBlock, throughBlock);
	registerBoolSetting("Visualize", &visualize, visualize);
}

BowAimbot::~BowAimbot() {
}

const char* BowAimbot::getModuleName() {
	return ("BowAimbot");
}

static std::vector<C_Entity*> targetList;

void findTargets(C_Entity* currentEntity, bool isRegularEntitie) {
	if (!Target::isValidTarget(currentEntity))
		return;

	static auto bowAimbotMod = moduleMgr->getModule<BowAimbot>();
	if (!bowAimbotMod->throughBlock && !g_Data.getLocalPlayer()->canSee(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 160) {
		targetList.push_back(currentEntity);
	}
}

void BowAimbot::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	targetList.clear();

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->getSelectedItemId() != 300)  // Bow in hand?
		return;

	if (!(GameData::isRightClickDown() && GameData::canUseMoveKeys())) // is aiming?
		return;

	g_Data.forEachEntity(findTargets);

	if (!targetList.empty()) {
		if (priority.selected == 0) {
			struct DistanceCompare {
				bool operator()(C_Entity* lhs, C_Entity* rhs) {
					C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
					return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
				}
			};

			std::sort(targetList.begin(), targetList.end(), DistanceCompare());
		}
		else if (priority.selected == 1) {
			struct AngleCompare {
				bool operator()(C_Entity* lhs, C_Entity* rhs) {
					C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
					vec2_t angle = localPlayer->getPos()->CalcAngle(*lhs->getPos());
					vec2_t angle2 = localPlayer->getPos()->CalcAngle(*rhs->getPos());
					vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
					vec2_t appl2 = angle2.sub(localPlayer->viewAngles).normAngles();
					//logF("appl = %f appl2 = %f", abs(appl.y), abs(appl2.y));
					return abs(appl.y) < abs(appl2.y);
				}
			};

			std::sort(targetList.begin(), targetList.end(), AngleCompare());
		}

		vec3_t origin = *localPlayer->getPos();
		C_Entity* entity = targetList[0];
		vec3_t pos = entity->aabb.centerPoint();

		if (predict) {
			vec3_t velocity = entity->getPos()->sub(*entity->getPosOld());
			velocity.x *= origin.dist(pos) / 2.f;
			velocity.z *= origin.dist(pos) / 2.f;
			pos = pos.add(velocity);
		}

		if (visualize)
			DrawUtils::drawBox(pos.sub(0.5), pos.add(0.5), 0.3f, true);

		pos = pos.sub(origin);
		float yaw = (atan2f(pos.z, pos.x) * DEG_RAD) - 90;
		float len = pos.magnitudexz();
		constexpr float g = 0.002f;  // nukkit = 0.012, some servers need different values
		float tmp = 1 - g * (g * (len * len) + 2 * pos.y);
		float pitch = DEG_RAD * -atanf((1 - sqrtf(tmp)) / (g * len));

		if (pitch < 89.9f && pitch > -89.9f) {
			angle = vec2_t(pitch, yaw);
			vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
			appl.x = -appl.x;
			if ((appl.y < FOV && appl.y > -FOV)) {
				if (silent) {
					C_MovePlayerPacket packet(localPlayer, *g_Data.getLocalPlayer()->getPos());
					packet.pitch = angle.x;
					packet.yaw = angle.y;
					packet.headYaw = angle.y;
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
				}
				else {
					appl = appl.div(7);  // Smooth dat boi

					localPlayer->applyTurnDelta(&appl);
				}
			}
		}
	}
}

void BowAimbot::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && silent) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}
