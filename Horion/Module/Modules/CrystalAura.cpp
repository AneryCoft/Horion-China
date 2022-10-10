#include "CrystalAura.h"

CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Place and destroys End Crystals.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("Destroy Range", &destroyRange, destroyRange, 3.f, 7.f);
	registerFloatSetting("Place Range", &placeRange, placeRange, 4.5f, 10.f);
	registerIntSetting("MultiPlace Range", &multiPlaceRange, multiPlaceRange, 1, 10);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerBoolSetting("AutoSelect", &autoSelect, autoSelect);
	registerBoolSetting("AutoPlace", &autoPlace, autoPlace);
	registerBoolSetting("Preview", &preview, preview);
}

CrystalAura::~CrystalAura() {
}

const char* CrystalAura::getModuleName() {
	return ("CrystalAura");
}

static std::vector<C_Entity*> playerList;
static std::vector<C_Entity*> crystalList;

void findPlayerAndCrystal(C_Entity* currentEntity, bool isRegularEntity) {
	static auto crystalAuraMod = moduleMgr->getModule<CrystalAura>();

	if (currentEntity == nullptr)
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	float distance = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (currentEntity->getEntityTypeId() == 71) {
		if (distance > crystalAuraMod->destroyRange)
			return;

		crystalList.push_back(currentEntity);
		return;
	}

	if (Target::isValidTarget(currentEntity)) {
		if (distance > crystalAuraMod->placeRange)
			return;

		playerList.push_back(currentEntity);
		return;
	}
}

bool space = true;
vec3_t _pos;
bool canPlaceCrystal(vec3_ti* pos) {
	space = true;
	_pos = pos->toVec3t();

	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (ent->aabb.intersects(AABB(_pos, _pos.add(1.f)))) {
			space = false;
			return;
		}
		});

	return space;
} //检查是否有实体挡住方块位置

void CrystalAura::placeCrystal(C_GameMode* gm, vec3_t* pos) {
	bool hasBestPos = false;

	for (int x = (int)pos->x - multiPlaceRange; x < pos->x + multiPlaceRange; x++) {
		for (int z = (int)pos->z - multiPlaceRange; z < pos->z + multiPlaceRange; z++) {
			for (int y = (int)pos->y - multiPlaceRange; y < pos->y + multiPlaceRange; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				vec3_ti upperBlockPos = vec3_ti(x, y + 1, z); //实际上基岩版的末地水晶需要两格空气才能放置 这样做是为了兼容一些互通服
				C_Block* block = gm->player->region->getBlock(blockPos);
				C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
				if (gm->player->getPos()->dist(blockPos.toVec3t()) > placeRange)
					continue;
				if (block != nullptr) {
					short blockId = block->toLegacy()->blockId;
					short upperBlockId = upperBlock->toLegacy()->blockId;
					if ((blockId == 49 || blockId == 7) && upperBlockId == 0 && canPlaceCrystal(&upperBlockPos)) {  //查找可以放末地水晶的方块
						if (mode.selected == 0) {
							if (!hasBestPos) {
								hasBestPos = true;
								bestPos = blockPos;
							}
							else if (blockPos.toVec3t().dist(*pos) < bestPos.toVec3t().dist(*pos)) {
								bestPos = blockPos;
							} //寻找距离目标最近的位置
						}
						else {
							bool idk = true;
							gm->buildBlock(&blockPos, 0, idk);
						}
					}
				}
			}
		}
	}

	if (mode.selected == 0 && hasBestPos) {
		bool idk = true;
		gm->buildBlock(&bestPos, 0, idk);
	}
} //放置末地水晶

void CrystalAura::destroyCrystal() {
	if (mode.selected == 0) {
		g_Data.getLocalPlayer()->swingArm();
		g_Data.getCGameMode()->attack(crystalList[0]);
	}
	else {
		for (auto& i : crystalList) {
			g_Data.getLocalPlayer()->swingArm();
			g_Data.getCGameMode()->attack(i);
		}
	}
} //破坏末地水晶

void CrystalAura::selectCrystal() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inventory = supplies->inventory;
	int emptySlot = 0;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (i < 9) {
			if (stack->item != nullptr) {
				if ((*stack->item)->itemId == 637) {
					supplies->selectedHotbarSlot = i;
					return;
				}
			}
			else {
				emptySlot = i;
			}
		}
		else {
			if (stack->item != nullptr && (*stack->item)->itemId == 637) {
				inventory->swapSlots(i, emptySlot);
				supplies->selectedHotbarSlot = emptySlot;
				return;
			}
		}
	}
}

void CrystalAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) {
		setEnabled(false);
		return;
	}

	tick = 0;
	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
}

void CrystalAura::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	}
}

void CrystalAura::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer == nullptr)
		return;

	playerList.clear();
	crystalList.clear();
	g_Data.forEachEntity(findPlayerAndCrystal);

	tick++;

	if (tick == delay) {
		if (!playerList.empty()) {
			if (autoSelect) {
				selectCrystal();
			}
		}
		return;
	}

	if (tick == delay + 1) {
		if (!playerList.empty()) {
			if (autoPlace && g_Data.getLocalPlayer()->getSelectedItemId() == 637) {  //endcrystal
				std::sort(playerList.begin(), playerList.end(), [](const C_Entity* lhs, const C_Entity* rhs) {
					vec3_t localPos = *g_Data.getLocalPlayer()->getPos();
					C_Entity* current = const_cast<C_Entity*>(lhs);
					C_Entity* other = const_cast<C_Entity*>(rhs);
					return (*current->getPos()).dist(localPos) < (*other->getPos()).dist(localPos);
					}); //距离优先

				if (mode.selected == 0) {
					placeCrystal(gm, playerList[0]->getPos());
					/*auto playerLevel = g_Data.getLocalPlayer()->level;
					if (playerLevel->getEntity() == nullptr && playerLevel->rayHitType == 0) {
						placeCrystal(gm, &playerLevel->block.toFloatVector());
					}*/
				}
				else {
					for (auto& i : playerList) {
						placeCrystal(gm, i->getPos());
					}
				}
			}
		}
		return;
	}

	if (tick == delay + 3) {
		if (!crystalList.empty()) {
			destroyCrystal();
		}
		tick = 0;
	}
}

void CrystalAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (mode.selected == 1 ||
		!preview ||
		!autoPlace ||
		!g_Data.canUseMoveKeys() ||
		g_Data.getLocalPlayer() == nullptr)
		return;

	if (!playerList.empty()) {
		DrawUtils::setColor(.75f, .25f, .5f, 1.f);
		DrawUtils::drawBox(bestPos.toVec3t().add(0.f, 1.5f, 0.f), bestPos.add(1).toVec3t().add(0.f, 1.5f, 0.f), .3f);
	}
	/*auto playerLevel = g_Data.getLocalPlayer()->level;
	if (playerLevel != nullptr)
		if (playerLevel->getEntity() == nullptr && playerLevel->rayHitType == 0)
			if (g_Data.getLocalPlayer()->region->getBlock(playerLevel->block)->toLegacy()->blockId == 49 ||
				g_Data.getLocalPlayer()->region->getBlock(playerLevel->block)->toLegacy()->blockId == 7) {
				DrawUtils::setColor(.75f, .25f, .5f, 1.f);
				DrawUtils::drawBox(playerLevel->block.toVec3t().add(0.f, 1.5f, 0.f),
					playerLevel->block.add(1).toVec3t().add(0.f, 1.5f, 0.f), .3f);
			}*/
}