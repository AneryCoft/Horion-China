#include "Breaker.h"

Breaker::Breaker() : IModule(VK_NUMPAD9, Category::WORLD, "Destroys certain blocks and entities around you.") {
	action = SettingEnum(this)
		.addEntry(EnumEntry("Destroy", 0))
		.addEntry(EnumEntry("Build", 1));
	registerEnumSetting("Action", &action, 0);
	registerIntSetting("Range", &range, range, 3, 10);
	registerFloatSetting("Delay", &delay, delay, 0.f, 1000.f);
	registerFloatSetting("lineWidth", &thick, thick, 0.1f, 0.8f);
	registerBoolSetting("TargetESP", &targetEsp, targetEsp);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("ThroughBlock", &throughBlock, throughBlock);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("DragonEggs", &eggs, eggs);
	registerBoolSetting("Cakes", &cakes, cakes);
	registerBoolSetting("Chests", &chests, chests);
	registerBoolSetting("Barrels", &barrels, barrels);
	registerBoolSetting("RedStone", &redStone, redStone);
	registerBoolSetting("Treasures", &treasures, treasures);       //用于The Hive的Treasure Wars
	registerBoolSetting("Bed Item", &lifeboatBeds, lifeboatBeds);  //Lifeboat的实体床
	registerBoolSetting("Core", &core, core);                      //用于Galaxite的Core Wars
}

Breaker::~Breaker() {
}

const char* Breaker::getModuleName() {
	return ("Breaker");
}

void Breaker::findBlocks() {
	vec3_t* pos = g_Data.getLocalPlayer()->getPos();

	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				short blockId = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;
				
				if ((blockId == 26 && beds) ||
					((blockId == 122 || blockId == 1000) && eggs) ||
					(blockId == 92 && cakes) ||
					(blockId == 54 && chests) ||
					(blockId == 458 && barrels) ||
					((blockId == 73 || blockId == 74) && redStone && g_Data.getLocalPlayer()->getHealth() < 15)) {
					if (!throughBlock) {
						static std::array<vec3_t, 6> faceList = {
						vec3_t(0.f, -0.5f, 0.f), //0
						vec3_t(0.f, 0.5f, 0.f), //1
						vec3_t(0.f, 0.f, -0.5f), //2
						vec3_t(0.f, 0.f, 0.5f), //3
						vec3_t(-0.5f, 0.f, 0.f), //4
						vec3_t(0.5f, 0.f, 0.f) //5
						};

						for (int i = 0; i < 6; i++) {
							if (g_Data.getLocalPlayer()->canSee(blockPos.toVec3t().add(0.5f).add(faceList[i]))) {
								blockList.emplace_back(blockPos, blockId, i);
								break;
							}
						}
					}
					else {
						blockList.emplace_back(blockPos, blockId, 1);
					}
				}
			}
		}
	}
}

void findEntityBed(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto breakerMod = moduleMgr->getModule <Breaker>();

	if (currentEntity == nullptr)
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist > breakerMod->range)
		return;

	std::string entityName = currentEntity->getNameTag()->getText();

	if (
		/*(currentEntity->getEntityTypeId() == 256 &&
			((currentEntity->height > 0.79f && currentEntity->height < 0.81f)
				&& (currentEntity->width > 0.79f && currentEntity->width < 0.81f) //小型宝藏(0.8*0.8)
				|| (currentEntity->height > 2.39f && currentEntity->height < 2.41f)
				&& (currentEntity->width > 2.39f && currentEntity->width < 2.41f)) //12V12中的大型宝藏(2.4*2.4)
			&& breakerMod->treasures)*/
		(!strcmp(currentEntity->getType()->getText(), "treasure") && breakerMod->treasures)
		|| (entityName.find("'s Bed") != std::string::npos && breakerMod->lifeboatBeds)
		|| ((currentEntity->height > 1.24f && currentEntity->height < 1.26f)
			&& (currentEntity->width > 0.39 && currentEntity->width < 0.41) //1.25*0.4 
			&& breakerMod->core)
		) {
		breakerMod->entityBedList.push_back(currentEntity);
	}
}

void Breaker::selectPickaxe() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inventory = supplies->inventory;

	for (int n = 0; n < 36; n++) {
		C_ItemStack* stack = inventory->getItemStack(n);
		if (stack->item != nullptr && stack->getItem()->isPickaxe()) {
			prevSlot = supplies->selectedHotbarSlot;
			supplies->selectedHotbarSlot = n;
			return;
		}
	}
}

void Breaker::onTick(C_GameMode* gm) {
	auto localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	blockList.clear();
	findBlocks();

	entityBedList.clear();
	g_Data.forEachEntity(findEntityBed);

	if (blockList.empty() && entityBedList.empty())
		shouldRotation = false;

	if (!blockList.empty()) {
		vec3_t localPos = *g_Data.getLocalPlayer()->getPos();

		std::sort(blockList.begin(), blockList.end(), [localPos](auto& lhs, auto& rhs) {
			return localPos.dist(std::get<vec3_ti>(lhs).toFloatVector()) < localPos.dist(std::get<vec3_ti>(rhs).toFloatVector());
			}); //距离优先 

		vec3_ti blockPos = std::get<vec3_ti>(blockList[0]);

		if (rotations) {
			angle = localPos.CalcBlockAngle(blockPos.toVec3t(), std::get<uint8_t>(blockList[0]));
			shouldRotation = true;
		}

		if (delayTime.hasTimedElapsed(delay, true)) {
			bool isRedStoneOre = std::get<short>(blockList[0]) == 73 || std::get<short>(blockList[0]) == 74;
			if (isRedStoneOre)
				selectPickaxe();

			if (action.selected == 0) {
				bool isDestroyedOut = false;
				gm->startDestroyBlock(blockPos, std::get<uint8_t>(blockList[0]), isDestroyedOut);
				gm->destroyBlock(&blockPos, std::get<uint8_t>(blockList[0]));
				gm->stopDestroyBlock(blockPos);
			}
			else if (action.selected == 1) {
				gm->buildBlock(&blockPos, std::get<uint8_t>(blockList[0]), true);
			}

			if (isRedStoneOre)
				localPlayer->getSupplies()->selectedHotbarSlot = prevSlot;
		}
	}

	if (!entityBedList.empty()) {
		if (rotations) {
			angle = localPlayer->getPos()->CalcAngle(entityBedList[0]->aabb.centerPoint());
			shouldRotation = true;
		}
		if (delayTime.hasTimedElapsed(delay, true)) {
			localPlayer->swingArm();
			gm->attack(entityBedList[0]);
		}
	}
}

void Breaker::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())
		return;

	if (targetEsp) {
		if (!blockList.empty()) {
			vec3_t renderPos = (std::get<vec3_ti>(blockList[0])).toVec3t();
			short blockId = std::get<short>(blockList[0]);

			if (blockId == 26) {
				DrawUtils::setColor(133 / 255.f, 16 / 255.f, 14 / 255.f, 1);
				DrawUtils::drawBox(renderPos, renderPos.add(1.f), thick, false);
			} //床 
			else if (blockId == 122 || blockId == 1000/*z_cubecraftcustom_chest_block ID:1000*/) {
				DrawUtils::setColor(45 / 255.f, 1 / 255.f, 51 / 255.f, 1);
				DrawUtils::drawBox(renderPos, renderPos.add(1.f), thick, false);
			} //龙蛋 
			else if (blockId == 92) {
				DrawUtils::setColor(199 / 255.f, 97 / 255.f, 36 / 255.f, 1);
				DrawUtils::drawBox(renderPos, renderPos.add(1.f), thick, false);
			} //蛋糕 
			else if (blockId == 54) {
				DrawUtils::setColor(164 / 255.f, 114 / 255.f, 39 / 255.f, 1);
				DrawUtils::drawBox(renderPos, renderPos.add(1.f), thick, false);
			} //箱子 
			else if (blockId == 458) {
				DrawUtils::setColor(85 / 255.f, 58 / 255.f, 31 / 255.f, 1);
				DrawUtils::drawBox(renderPos, renderPos.add(1.f), thick, false);
			} //木桶 
			else if (blockId == 73 || blockId == 74) {
				DrawUtils::setColor(255 / 255.f, 0 / 255.f, 0 / 255.f, 1);
				DrawUtils::drawBox(renderPos, renderPos.add(1.f), thick, false);
			} //红石矿石和发光的红石矿石 
		}

		if (!entityBedList.empty()) {
			if (entityBedList[0] != nullptr) {
				DrawUtils::setColor(0.9f, 0.9f, 0.9f, 0.6f);
				DrawUtils::drawEntityBox(entityBedList[0], fmax(thick, 1 / fmax(1, g_Data.getLocalPlayer()->getPos()->dist(*entityBedList[0]->getPos()))));
			}
		}
	}
}

void Breaker::onPlayerTick(C_Player* player) {
	if (rotations && shouldRotation) {
		player->pitch = angle.x;
		player->bodyYaw = angle.y;
		player->yawUnused1 = angle.y;
	}
}

void Breaker::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (rotations && shouldRotation) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
		/*
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->pitch = angle.x;
			authInputPacket->yawUnused = angle.y;
			authInputPacket->yaw = angle.y;
		}
		*/
	}
}
