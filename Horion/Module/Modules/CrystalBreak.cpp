#pragma once
#include "CrystalUtilsJTWD.h"

#include "../ModuleManager.h"

CrystalBreak::CrystalBreak() : IModule(0, Category::COMBAT, "comes as a set with CrystalConfig ;)") {
	registerBoolSetting("AntiWeak(shitty)", &this->antiWeakness, this->antiWeakness);
	registerIntSetting("Delay (ticks)", &this->breakdelay, this->breakdelay, 0, 8);

	registerIntSetting("Break Range", &this->breakRange, this->breakRange, 0, 12);

	registerBoolSetting("Break All", &this->breakAll, this->breakAll);

	registerIntSetting("Thru Walls", &this->breakWalls, this->breakWalls, 0, 10);
	registerFloatSetting("Post Walls", &this->postBWalls, this->postBWalls, 0.f, 10.f);

	registerFloatSetting("Minimum Health", &this->breakHealth, this->breakHealth, 0.f, 20.f);
	registerFloatSetting("Max Self Dmg", &this->BmaxSelfDmg, this->BmaxSelfDmg, 0.f, 20.f);

	registerBoolSetting("Render", &this->renderBreaking, this->renderBreaking);

	rotateBreak = SettingEnum(this)
		.addEntry(EnumEntry("none", 0))
		.addEntry(EnumEntry("normal", 1))
		.addEntry(EnumEntry("silent", 2));
	registerEnumSetting("Rotations", &this->rotateBreak, 0);

	//registerBoolSetting("Config Helper", &this->configHelper, this->configHelper);
}

CrystalBreak::~CrystalBreak() {};

const char* CrystalBreak::getModuleName() {
	return ("CrystalAuraNew");
}

static std::vector<C_Entity*> crystalList;
bool crystalFinder(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() != 71) return false;                   // not endcrystal
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org

	float dist = (curEnt->eyePos0).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<CrystalBreak>()->breakRange) {
		crystalList.push_back(curEnt);
		return true;
	}
	return false;
}


bool cmpCC(C_Entity* E1, C_Entity* E2) {
	return g_Data.getLocalPlayer()->getHumanPos().dist(E1->eyePos0) < g_Data.getLocalPlayer()->getHumanPos().dist(E2->eyePos0);
}

void CrystalBreak::sortCrystals() {
	std::vector<C_Entity*> rtn;

	if (g_Data.getLocalPlayer()->getHealth() <= breakHealth) {
		crystalList.clear();
		return;
	}

	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4"));

	for (C_Entity* aCrystal : crystalList) {
		if (aCrystal->getEntityTypeId() == 71) {
			for (CrystalInfo& c : moduleMgr->getModule<CrystalPlace>()->CJTWDPlaceArr) {
				vec3_t cPos = aCrystal->eyePos0.floor();

				if (!breakAll) {  // ensure that the crystals are the ones placed by you
					vec3_t place = c.CPlacements.toPlace.floor();

					if (cPos == place) {  // crystal is on *that* locaation
						rtn.push_back(aCrystal);
					}
				}

				// place location obstructed; do wall range
				if (getSeenPercent(g_Data.getLocalPlayer()->region, cPos, g_Data.getLocalPlayer()->aabb) == 0.f && breakWalls != 10) {
					if (breakWalls == 0)  // walls completely disabled, might as well save on processing time by not going through
						continue;

					lineResults lineRes = countBlksAlongLine(g_Data.getLocalPlayer()->eyePos0, cPos.add(0.5f));
					int amtOfShitInBtwn = lineRes.blockCount;

					if (amtOfShitInBtwn >= breakWalls)  // theres too much shit in between
						continue;

					vec3_t lastKnownSolid = lineRes.lastSolidBlock;
					if (lastKnownSolid.dist(cPos) > postBWalls)  // shit is too far away from wall
						continue;

					rtn.push_back(aCrystal);
				}

				if (c.CPlacements.selfDmg < BmaxSelfDmg)
					rtn.push_back(aCrystal);
			}
		}
	}

	std::sort(rtn.begin(), rtn.end(), cmpCC);

	crystalList.clear();
	crystalList = rtn;
	return;
}

void CrystalBreak::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;

	crystalList.clear();

	auto CPlace = moduleMgr->getModule<CrystalPlace>();
	if (!CPlace->isEnabled())
		CPlace->setEnabled(true);

	return;
}

void CrystalBreak::onDisable() {
	auto CPlace = moduleMgr->getModule<CrystalPlace>();
	if (CPlace->isEnabled())
		CPlace->setEnabled(false);

	return;
}

void CrystalBreak::findAFuckingWeapon() {

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	oldSlot = supplies->selectedHotbarSlot;

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

void CrystalBreak::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	crystalList.clear();

	g_Data.forEachEntity(crystalFinder);

	sortCrystals();
	breakArrEmpty = crystalList.empty();
	if (breakArrEmpty) return;

	Mdel++;

	if (g_Data.getLocalPlayer()->getSelectedItemId() == 300 || g_Data.getLocalPlayer()->getSelectedItemId() == 316 || g_Data.getLocalPlayer()->getSelectedItemId() == 318 || g_Data.getLocalPlayer()->getSelectedItemId() == 319 || g_Data.getLocalPlayer()->getSelectedItemId() == 604 || g_Data.getLocalPlayer()->getSelectedItemId() == 607 || g_Data.getLocalPlayer()->getSelectedItemId() == 606)
		return;

	aSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (moduleMgr->getModule<CrystalPlace>()->switchType.GetSelectedEntry().GetValue() == 3) {
		if ((g_Data.getLocalPlayer()->level->hasEntity() != 0) && GameData::isLeftClickDown())
			return;
		if (g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot != aSlot)
			return;
	}

	if (Mdel >= breakdelay) {
		if (antiWeakness) {
			findAFuckingWeapon();
			return;
		}

		if (breakAll) {
			for (C_Entity* EC_ent : crystalList) {
				if (EC_ent->isAlive() && EC_ent != nullptr) {

					if (rotateBreak.GetSelectedEntry().GetValue() == 1 || rotateBreak.GetSelectedEntry().GetValue() == 2) {
						rotAngleB = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(EC_ent->eyePos0);
					}

					g_Data.getCGameMode()->attack(EC_ent);
				}
			}
		}
		else if (!breakAll) {
			if (crystalList[0]->isAlive() && crystalList[0] != nullptr) {

				if (rotateBreak.GetSelectedEntry().GetValue() == 1 || rotateBreak.GetSelectedEntry().GetValue() == 2) {
					rotAngleB = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(crystalList[0]->eyePos0);
				}

				g_Data.getCGameMode()->attack(crystalList[0]);
			}
		}
		Mdel = 0;
		if (antiWeakness) {
			g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = oldSlot;
		}
	}

	return;
}


void CrystalBreak::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!crystalList.empty() && renderBreaking && g_Data.isInGame() && g_Data.canUseMoveKeys() && g_Data.getLocalPlayer() != nullptr) {
		for (C_Entity* i : crystalList) {
			DrawUtils::setColor(1.f, 0.f, .2f, 0.8f);
			DrawUtils::drawBox(i->eyePos0.sub(.5f, 0, .5f),
				i->eyePos0.add(.5f, 1, .5f), .1f);
		}
	}
}

void CrystalBreak::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && rotateBreak.GetSelectedEntry().GetValue() == 2) {  // silent rotations
		if (!breakArrEmpty) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = rotAngleB;
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}

