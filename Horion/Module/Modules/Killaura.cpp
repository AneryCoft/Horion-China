#include "Killaura.h"
#include "../../../Memory/Hooks.h"
#include <random>

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically.") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1))
		.addEntry(EnumEntry("Switch", 2));
	registerEnumSetting("Mode", &mode, 0);
	rotations = (*new SettingEnum(this))
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Packet", 1))
		.addEntry(EnumEntry("Vanilla", 2))
		.addEntry(EnumEntry("Actual", 3));
	registerEnumSetting("Rotations", &rotations, 0);
	registerFloatSetting("Range", &range, range, 3.f, 10.f);
	registerIntSetting("MaxCPS", &maxCPS, maxCPS, 1, 20);
	registerIntSetting("MinCPS", &minCPS, minCPS, 1, 20);
	registerFloatSetting("Switch Delay", &switchDelay, switchDelay, 1.f, 1000.f);
	registerFloatSetting("Yaw Offset", &yawOffset, yawOffset, 0.f, 10.f);
	registerFloatSetting("Pitch Offset", &pitchOffset, pitchOffset, 0.f, 10.f);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerBoolSetting("Hurttime", &hurttime, hurttime);
	registerBoolSetting("AutoWeapon", &autoweapon, autoweapon);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

static std::vector<C_Entity*> targetList;
void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!currentEntity->checkNameTagFunc())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64) // item
			return;
		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;
		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;
	}
	else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
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

void Killaura::onGetPickRange() {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr || !localPlayer->isAlive())
		return;

	static bool swing = !moduleMgr->getModule<NoSwing>()->isEnabled();

	targetList.clear();

	g_Data.forEachEntity(findEntity);

	targetListEmpty = targetList.empty();

	if (!targetList.empty()) {
		if (autoweapon)
			findWeapon();

		if (mode.selected != 2 || switchTarget >= targetList.size()) {
			switchTarget = 0;
		}

		if (rotations.selected != 0) {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[switchTarget]->getPos());
			static std::default_random_engine engine;
			angle.x += std::uniform_real_distribution<float>(0.f, pitchOffset)(engine);
			angle.y += std::uniform_real_distribution<float>(0.f, yawOffset)(engine);
		}
		if (rotations.selected == 2) {
			localPlayer->setRot(angle);
		}

		CPS = RandomNumber(minCPS, maxCPS);
		//CPS = rand() % (maxCPS - minCPS + 1) + minCPS;
		if (TimerUtil::hasTimedElapsed(1000.f / CPS, true)) {
			if (rotations.selected == 1) {
				if (localPlayer->velocity.squaredxzlen() < 0.01) {
					C_MovePlayerPacket packet(localPlayer, *localPlayer->getPos());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);  //不动的时候Packet转头也能工作
				}
			}

			switch (mode.selected) {
			case 0:
				if (!(targetList[0]->damageTime > 1 && hurttime)) {
					if (swing)
						localPlayer->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
				}
				break;
			case 1:
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						if (swing)
							localPlayer->swing();
						g_Data.getCGameMode()->attack(i);
					}
				}
				break;
			case 2:
				if (!(targetList[switchTarget]->damageTime > 1 && hurttime)) {
					if (swing)
						localPlayer->swing();
					g_Data.getCGameMode()->attack(targetList[switchTarget]);
				}
			}
		}

		if (mode.selected == 2) {
			if (TimerUtil::hasTimedElapsed(1000.f / CPS, true)) {
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
}

void Killaura::onSendPacket(C_Packet* packet) {
	if (rotations.selected == 1) {
		if (!targetList.empty()) {
			if (packet->isInstanceOf<C_MovePlayerPacket>()) {
				auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
			if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
				auto* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
				authInputPacket->pitch = angle.x;
				authInputPacket->yawUnused = angle.y;
				authInputPacket->yaw = angle.y;
			}
		}
	}
}
