#include "Killaura.h"

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically.") { //hoiron hud显示不下这么多选项
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1))
		.addEntry(EnumEntry("Switch", 2));
	registerEnumSetting("Mode", &mode, 0);
	rotations = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Packet", 1))
		.addEntry(EnumEntry("Vanilla", 2))
		.addEntry(EnumEntry("Actual", 3));
	registerEnumSetting("Rotations", &rotations, 0);
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Angle", 1))
		.addEntry(EnumEntry("Health", 2))
		.addEntry(EnumEntry("Armor", 3));
	//.addEntry(EnumEntry("Threaten", 3));
	registerEnumSetting("Priority", &priority, 0);
	/*registerFloatSetting("Max Range", &maxRange, maxRange, 1.5f, 10.f);
	registerFloatSetting("Min Range", &minRange, minRange, 1.5f, 10.f);*/
	registerFloatSetting("Attack Range", &range, range, 3.f, 10.f);
	registerFloatSetting("Swing Range", &swingRange, swingRange, 3.f, 15.f);
	registerFloatSetting("FOV", &FOV, FOV, 15.f, 360.f);
	registerIntSetting("Max CPS", &maxCPS, maxCPS, 1, 20);
	registerIntSetting("Min CPS", &minCPS, minCPS, 1, 20);
	registerFloatSetting("Switch Delay", &switchDelay, switchDelay, 1.f, 1000.f);
	registerFloatSetting("Rotation Speed", &rotationSpeed, rotationSpeed, 10.f, 180.f);
	registerBoolSetting("RandomPitch", &randomPitch, randomPitch);
	registerBoolSetting("RandomYaw", &randomYaw, randomYaw);
	registerBoolSetting("Attack Mob", &attackMob, attackMob);
	registerBoolSetting("ThroughBlock", &throughBlock, throughBlock);
	registerBoolSetting("AutoDisable", &autoDisable, autoDisable);
	registerBoolSetting("Hurttime", &hurttime, hurttime);
	registerBoolSetting("AutoWeapon", &autoweapon, autoweapon);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

static std::vector<C_Entity*> targetList;
static void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (killauraMod->attackMob) {
		if (currentEntity == nullptr)
			return;

		if (currentEntity == g_Data.getLocalPlayer())
			return;

		if (!currentEntity->isAlive())
			return;

		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;
	}
	else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	vec2_t czxPos(currentEntity->getPos()->x, currentEntity->getPos()->z);
	vec2_t lxzPos(g_Data.getLocalPlayer()->getPos()->x, g_Data.getLocalPlayer()->getPos()->z);
	constexpr float r = 180.f / PI;

	if (killauraMod->FOV < 360.f) {
		if (abs(g_Data.getLocalPlayer()->pitch) > 60.f) { //当视角绝对值大于60度时，计算目标中点与准星中点的距离偏移。其他情况仅计算yaw的偏移
			if (abs(g_Data.getLocalPlayer()->viewAngles.normAngles().sub(g_Data.getLocalPlayer()->getPos()->CalcAngle(*currentEntity->getPos()).normAngles()).normAngles().magnitude()) > killauraMod->FOV)
				return;
		}
		else {

			float zxdist = sqrt((lxzPos.x - czxPos.x) * (lxzPos.x - czxPos.x) + (lxzPos.y - czxPos.y) * (lxzPos.y - czxPos.y));
			float asinyew = asin((czxPos.x - lxzPos.x) / zxdist) * r;
			float acosyew = acos((czxPos.y - lxzPos.y) / zxdist) * r;
			float powce;
			if (asinyew > 0.f) {
				if (acosyew > 90.f)  //1
					powce = -180.f + asinyew;
				else  //2
					powce = -asinyew;
			}
			else {
				if (acosyew > 90.f)  //4
					powce = 180.f + asinyew;
				else  //3
					powce = -asinyew;
			}
			float powceH = powce + killauraMod->FOV / 2.f;
			float powceL = powce - killauraMod->FOV / 2.f;

			if (powceH > 180.f) {
				if (!(g_Data.getCGameMode()->player->yaw >= powceL || g_Data.getCGameMode()->player->yaw <= powceH - 360.f))
					return;
			}
			else if (powceL < -180.f) {
				if (!(g_Data.getCGameMode()->player->yaw >= powceL + 360.f || g_Data.getCGameMode()->player->yaw <= powceH))
					return;
			}
			else if (!(g_Data.getCGameMode()->player->yaw >= powceL && g_Data.getCGameMode()->player->yaw <= powceH))
				return;
		}
	}

	if (!killauraMod->throughBlock && !g_Data.getLocalPlayer()->canSee(currentEntity))
		return; //穿墙检测

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (killauraMod->swingRange < killauraMod->range)
		killauraMod->swingRange = killauraMod->range;

	if (!killauraMod->canswing && dist < killauraMod->swingRange)
		killauraMod->canswing = true;

	if (dist < killauraMod->range)
		targetList.push_back(currentEntity);

}

void Killaura::selectedWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

/*
float Killaura::randomFloat(float min, float max) {
	static std::default_random_engine random(time(nullptr));//注入时的时间作为初始种子
	return std::uniform_real_distribution<float>(min, max)(random);
}*/

struct Distance {
	bool operator()(C_Entity* target, C_Entity* target2) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*target->getPos()).dist(*localPlayer->getPos()) < (*target2->getPos()).dist(*localPlayer->getPos());
	}
};

struct Angle {
	bool operator()(C_Entity* target, C_Entity* target2) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		vec2_t angle = localPlayer->getPos()->CalcAngle(*target->getPos()).normAngles();
		vec2_t angle2 = localPlayer->getPos()->CalcAngle(*target2->getPos()).normAngles();
		vec2_t appl = localPlayer->viewAngles.normAngles();

		auto normAngles = [](float num) noexcept -> float {
			while (num > 180.0f)
				num -= 360.0f;
			while (num < -180.0f)
				num += 360.0f;
			return num;
		};

		//当视角绝对值大于60度时，计算目标中点与准星中点的距离偏移，可以避免转头的pitch过大。其他情况仅计算yaw的偏移

		return abs(g_Data.getLocalPlayer()->pitch) > 60.f ? abs(appl.sub(angle).normAngles().magnitude()) < abs(appl.sub(angle2).normAngles().magnitude()) : abs(normAngles(angle.y - appl.y)) < abs(normAngles(angle2.y - appl.y));
	}
};

struct Health {
	bool operator()(C_Entity* target, C_Entity* target2) {
		return target->getAttribute(&HealthAttribute())->currentValue < target2->getAttribute(&HealthAttribute())->currentValue;
	}
};

struct Armor {
	bool operator()(C_Entity* target, C_Entity* target2) {
		float armorValue = 0.f, armorValue2 = 0.f;

		for (int i = 0; i < 4; i++) {
			C_ItemStack* stack = target->getArmor(i);
			C_ItemStack* stack2 = target2->getArmor(i);

			if (stack->item != nullptr) {
				armorValue += stack->getArmorValueWithEnchants();
			}
			if (stack2->item != nullptr) {
				armorValue2 += stack2->getArmorValueWithEnchants();
			}
		}
		return armorValue < armorValue2;
	}
};

/*
struct Threaten {
	bool operator()(C_Entity* target, C_Entity* target2) {
		const float health1 = target->getAttribute(&HealthAttribute())->currentValue;
		const float health2 = target2->getAttribute(&HealthAttribute())->currentValue;

		C_LocalPlayer* const localPlayer = g_Data.getLocalPlayer();
		const float dis1 = (*target->getPos()).dist(*localPlayer->getPos());
		const float dis2 = (*target2->getPos()).dist(*localPlayer->getPos());

		const float attackdamage1 = target->getAttribute(&AttackDamageAttribute())->currentValue;
		const float attackdamage2 = target2->getAttribute(&AttackDamageAttribute())->currentValue;

		//vec3_t velocity1 = target->getPos()->sub(*target->getPosOld());
		//vec3_t velocity2 = target2->getPos()->sub(*target2->getPosOld());

		uint16_t degree1 = 0;
		uint16_t degree2 = 0;


		degree1 += dis1 <= 3.f ? 100 : 80.f - 80.f * float(degree1) / 10.f; //距离
		degree2 += dis2 <= 3.f ? 100 : 80.f - 80.f * float(degree2) / 10.f;

		auto normAngles = [](float num) noexcept -> float {
			while (num > 180.0f)
				num -= 360.0f;
			while (num < -180.0f)
				num += 360.0f;
			return num;
		};

		const float targetyaw1 = abs(normAngles(target->getPos()->CalcAngle(*localPlayer->getPos()).normAngles().y - target->yaw)); //准星
		const float targetyaw2 = abs(normAngles(target2->getPos()->CalcAngle(*localPlayer->getPos()).normAngles().y - target2->yaw));

		degree1 += targetyaw1 <= 45.f ? 100 : 80.f - 80.f * targetyaw1 / 180.f;
		degree2 += targetyaw2 <= 45.f ? 100 : 80.f - 80.f * targetyaw2 / 180.f;

		float maxaattackdamage = attackdamage1 > attackdamage2 ? attackdamage1 : attackdamage2;

		if (maxaattackdamage > 0.f) {
			degree1 += 60.f * attackdamage1 / maxaattackdamage;
			degree2 += 60.f * attackdamage2 / maxaattackdamage;
		}
		return degree1 < degree2;
	}
};
*/

void Killaura::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	targetList.clear();

	if (localPlayer == nullptr || !localPlayer->isAlive())
		return;

	static auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	static auto towerMod = moduleMgr->getModule<Tower>();
	static auto breakerMod = moduleMgr->getModule<Breaker>();

	if (autoDisable &&
		(isDigging
			|| scaffoldMod->isEnabled()
			|| towerMod->isEnabled()
			|| (breakerMod->isEnabled() && !breakerMod->blockList.empty() && !breakerMod->entityBedList.empty())
			)
		)
		return;

	//static bool swing = !moduleMgr->getModule<NoSwing>()->isEnabled();

	canswing = false;

	g_Data.forEachValidEntity(findEntity);

	targetListEmpty = targetList.empty();

	if (!targetList.empty()) {
		if (autoweapon)
			selectedWeapon();

		switch (priority.selected) {
		case 0:
			sort(targetList.begin(), targetList.end(), Distance());
			break;
		case 1:
			sort(targetList.begin(), targetList.end(), Angle());
			break;
		case 2:
			sort(targetList.begin(), targetList.end(), Health());
			break;
		case 3:
			sort(targetList.begin(), targetList.end(), Armor());
		}

		if (mode.selected != 2 || switchTarget >= targetList.size()) {
			switchTarget = 0;
			lastTarget = nullptr;
		}

		if (lastTarget != nullptr && lastTarget != targetList[switchTarget]) {
			targetList[switchTarget] = lastTarget;
		}

		if (rotations.selected != 0) {
			/*vec3_t centrePos = {
		targetList[switchTarget]->getPos()->x,
		targetList[switchTarget]->aabb.lower.y + targetList[switchTarget]->height / 2,
		targetList[switchTarget]->getPos()->z
			};*/
			vec3_t centrePos = targetList[switchTarget]->aabb.centerPoint();
			vec3_t upperPos = targetList[switchTarget]->aabb.upper;
			vec3_t lowerPos = targetList[switchTarget]->aabb.lower;

			angle = localPlayer->getPos()->CalcAngle(centrePos);

			localAngle = vec2_t(localPlayer->pitch, localPlayer->bodyYaw);
			vec2_t appl = angle.sub(localAngle).normAngles();
			if (abs(appl.y) > rotationSpeed) {
				if (appl.y > 0.f) {
					localAngle.y += rotationSpeed;
					angle.y = localAngle.y;
				}
				else if (appl.y < 0.f) {
					localAngle.y -= rotationSpeed;
					angle.y = localAngle.y;
				}
				return;
			}
			else {
				//localAngle = localPlayer->viewAngles;
				if (randomPitch) {
					float upperPitch = localPlayer->getPos()->CalcAngle(upperPos).x;
					float lowerPitch = localPlayer->getPos()->CalcAngle(lowerPos).x;
					angle.x = randomFloat(upperPitch, lowerPitch);
					//clientMessageF("RandomPitch :%f", angle.x);

					if (randomYaw) {
						float upperYaw = localPlayer->getPos()->CalcAngle(upperPos).y;
						float lowerYaw = localPlayer->getPos()->CalcAngle(lowerPos).y;
						angle.y = randomFloat(upperYaw, lowerYaw);
						//clientMessageF("RandomYaw :%f", angle.y);
					}
					angle = angle.normAngles();
				}
			}

			/*if (pitchOffset > 0) {
				angle.x += randomFloat(pitchOffset, 0.f);
			}
			else if (pitchOffset < 0) {
				angle.x += randomFloat(0.f, pitchOffset);
			}

			if (yawOffset > 0) {
				angle.x += randomFloat(yawOffset, 0.f);
			}
			else if (yawOffset < 0) {
				angle.x += randomFloat(0.f, yawOffset);
			}*/

			/*
			if (rotationSpeed <100.f){
			auto turn = angle.sub(lastrotangle).normAngles();
			//turn.x = -turn.x;

			const float vspeed = rotationSpeed;
			turn.x /= (100.f - vspeed);
			turn.y /= (100.f - vspeed);
			if (turn.x >= 1 || turn.x <= -1)
				turn.div(abs(turn.x));
			if (turn.y >= 1 || turn.y <= -1)
				turn.div(abs(turn.y));
			angle = lastrotangle.add(turn);
			lastrotangle = angle;
			}*/
		}

		if (minCPS > maxCPS)
			minCPS = maxCPS;

		CPS = random(minCPS, maxCPS);

		//CPS = rand() % (maxCPS - minCPS + 1) + minCPS;
		if (attackTime.hasTimedElapsed(1000.f / CPS, true)) {
			/*
			if (rotations.selected == 1) {
				if (localPlayer->velocity.squaredxzlen() < 0.01) {
					C_MovePlayerPacket packet(localPlayer, *localPlayer->getPos());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);  //不动的时候Packet转头也能工作
				}
			}
			*/

			if (canswing && !hurttime) { //与hurttime的swing分开处理
				localPlayer->swing();
			}

			switch (mode.selected) {
			case 0:
				if (!(targetList[0]->damageTime > 1 && hurttime)) {
					localPlayer->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
				}
				break;
			case 1:
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						localPlayer->swing();
						g_Data.getCGameMode()->attack(i);
					}
				}
				break;
			case 2:
				if (!(targetList[switchTarget]->damageTime > 1 && hurttime)) {
					localPlayer->swing();
					g_Data.getCGameMode()->attack(targetList[switchTarget]);
				}
			}
		}

		if (mode.selected == 2) {
			if (switchTime.hasTimedElapsed(switchDelay, true)) {
				++switchTarget;
				lastTarget = nullptr;
			}
			else {
				lastTarget = targetList[switchTarget];
			}
		}
	}
	else {
		lastTarget = nullptr;
		if (canswing) {
			CPS = random(minCPS, maxCPS);
			if (attackTime.hasTimedElapsed(1000.f / CPS, true)) {
				if (!hurttime) {  //与hurttime的swing分开处理
					localPlayer->swing();
				}
			}
		}
	}
}

void Killaura::onEnable() {
	if (auto localPlayer = g_Data.getLocalPlayer(); localPlayer == nullptr)
		setEnabled(false);

	switchTime.resetTime();
	attackTime.resetTime();
	//计时器初始化
}

void Killaura::onDisable() {
	targetList.clear();
	lastTarget = nullptr;
	switchTarget = 0;
}

void Killaura::onPlayerTick(C_Player* player) {
	if (!targetList.empty()) {
		if (rotations.selected == 1) {
			player->pitch = angle.x;
			player->bodyYaw = angle.y;
			player->yawUnused1 = angle.y;
		}
		else if (rotations.selected == 2) {
			player->setRot(angle);
		}
	}
}

void Killaura::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (rotations.selected == 1) {
		if (!targetList.empty()) {
			if (packet->isInstanceOf<C_MovePlayerPacket>()) {
				auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
			/*if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
				auto* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
				authInputPacket->pitch = angle.x;
				authInputPacket->yawUnused = angle.y;
				authInputPacket->yaw = angle.y;
			}*/
		}
	}
}
