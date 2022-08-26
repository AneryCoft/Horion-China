#include "Killaura.h"
#include "../../../Memory/Hooks.h"
#include <random>
#include <unordered_map>

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically.") { //hoiron hud显示不下这么多选项。。。
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
		.addEntry(EnumEntry("Threaten", 3));
	registerEnumSetting("Priority", &priority, 0);
	registerFloatSetting("Max Range", &maxRange, maxRange, 1.5f, 10.f);
	registerFloatSetting("Min Range", &minRange, minRange, 1.5f, 10.f);
	registerFloatSetting("Swing Range", &swingRange, swingRange, 1.5f, 15.f);
	registerFloatSetting("FOV", &FOV, FOV, 15.f, 360.f);
	registerIntSetting("Max CPS", &maxCPS, maxCPS, 1, 20);
	registerIntSetting("Min CPS", &minCPS, minCPS, 1, 20);
	registerFloatSetting("Switch Delay", &switchDelay, switchDelay, 1.f, 1000.f);
	registerFloatSetting("Yaw Offset", &yawOffset, yawOffset, 0.f, 10.f);
	registerFloatSetting("Pitch Offset", &pitchOffset, pitchOffset, 0.f, 10.f);
	registerBoolSetting("Mob Aura", &isMobAura, isMobAura);
	registerBoolSetting("Behind Block", &attackBehindBlocks, attackBehindBlocks);
	registerBoolSetting("Disable dur mining", &DisabledDuringMining, DisabledDuringMining);
	registerBoolSetting("Playsound", &playsound, playsound);
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

	if (killauraMod->isMobAura) {
		if (currentEntity == nullptr)
			return;

		if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
			return;

		if (!currentEntity->checkNameTagFunc())
			return;

		if (!currentEntity->isAlive())
			return;

		//if (currentEntity->getEntityTypeId() == 66) // falling block
		//	return;

		//if (currentEntity->getEntityTypeId() == 69)  // XP
		//	return;

		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;

		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;

		//if (currentEntity->getEntityTypeId() == 64) // item
		//	return;

		//if (currentEntity->getEntityTypeId() == 80)  // Arrows
		//	return;

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
	//墙

	if (!killauraMod->attackBehindBlocks && !g_Data.getLocalPlayer()->canSee(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (!killauraMod->canswing && dist < killauraMod->swingRange)
		killauraMod->canswing = true;

	if (dist < Killaura::randomFloat(killauraMod->minRange, killauraMod->maxRange)) 
		targetList.push_back(currentEntity);

}

void Killaura::findWeapon() {
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

float Killaura::randomFloat(float min, float max) {
	static std::default_random_engine random(time(nullptr));//注入时的时间作为初始种子
	return std::uniform_real_distribution<float>(min, max)(random);
}

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
		return abs(normAngles(angle.y - appl.y)) < abs(normAngles(angle2.y - appl.y));
	}
};

struct Health {
	bool operator()(C_Entity* target, C_Entity* target2) {
		return target->getAttribute(&HealthAttribute())->currentValue < target2->getAttribute(&HealthAttribute())->currentValue;
	}
};

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

void Killaura::onGetPickRange() {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr || !localPlayer->isAlive() || DisabledDuringMining && isMining)
		return;

	if (minCPS > maxCPS)
		minCPS = maxCPS;

	if (minRange > maxRange)
		minRange = maxRange;

	if (swingRange < maxRange)
		swingRange = maxRange;

	static bool swing = !moduleMgr->getModule<NoSwing>()->isEnabled();

	targetList.clear();

	canswing = false;
	g_Data.forEachValidEntity(findEntity);

	targetListEmpty = targetList.empty();

	if (!targetList.empty()) {
		if (autoweapon)
			findWeapon();

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
			sort(targetList.begin(), targetList.end(), Threaten());
			break;
		default:
			break;
		}

		if (mode.selected != 2 || switchTarget >= targetList.size()) {
			switchTarget = 0;
		}

		if (rotations.selected != 0) {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[switchTarget]->getPos());
			angle.x += Killaura::randomFloat(0.f, pitchOffset);
			angle.y += Killaura::randomFloat(0.f, yawOffset);
		}

		if (rotations.selected == 2) {
			localPlayer->setRot(angle);
		}

		CPS = RandomNumber(minCPS, maxCPS);
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
			static LevelSoundEventPacket sounds;
			sounds.pos = *g_Data.getLocalPlayer()->getPos();
			sounds.sound = 42;

			if (canswing && swing && !hurttime) { //与hurttime的swing分开处理
				localPlayer->swing();
				if (playsound && strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0)
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&sounds);

			}

			switch (mode.selected) {
			case 0:
				if (!(targetList[0]->damageTime > 1 && hurttime)) {
					if (hurttime && swing) {
						localPlayer->swing();
						if (playsound && strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0)
							g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&sounds);
					}
					g_Data.getCGameMode()->attack(targetList[0]);
				}
				break;
			case 1:
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						if (hurttime && swing) {
							localPlayer->swing();	
							if (playsound && strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0)
								g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&sounds);
						}
						g_Data.getCGameMode()->attack(i);
					}
				}
				break;
			case 2:
				if (!(targetList[switchTarget]->damageTime > 1 && hurttime)) {
					if (hurttime && swing) {
						localPlayer->swing();	
						if (playsound && strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0)
							g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&sounds);
					}
					g_Data.getCGameMode()->attack(targetList[switchTarget]);
				}
			}
		}

		if (mode.selected == 2) {
			if (switchTime.hasTimedElapsed(switchDelay, true)) {
				++switchTarget;
			}
		}
	}
}

void Killaura::onPlayerTick(C_Player* player) {
	if (rotations.selected == 1) {
		if (!targetList.empty()) {
			player->pitch = angle.x;
			player->bodyYaw = angle.y;
			player->yawUnused1 = angle.y;
		}
	}
}

void Killaura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);

	if (g_Data.getRakNetInstance() != nullptr) {
		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0) {
			clientMessageF(u8"检测到您位于EaseCation测试服，已为您自动开启绕过CPS检测 ");
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

	if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0) {
		if (packet->isInstanceOf<LevelSoundEventPacket>()) {
			LevelSoundEventPacket* soundEventPacket = reinterpret_cast<LevelSoundEventPacket*>(packet);
			if (soundEventPacket->sound == 43 || soundEventPacket->sound == 42) //sound 42是空挥手时的数值 也会被计算进CPS 但是攻击的时候不发那个包 
				//soundEventPacket->sound = 0; 
				cancelSend = true;
		} //绕过EaseCation服务器CPS检测 
	}
}
