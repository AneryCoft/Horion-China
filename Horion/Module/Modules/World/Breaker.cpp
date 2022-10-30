#include "Breaker.h"

Breaker::Breaker() : IModule(VK_NUMPAD9, Category::WORLD, "Destroys certain blocks around you.") {
	registerIntSetting("Range", &range, range, 3, 10);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerFloatSetting("lineWidth", &thick, thick, 0.1f, 0.8f);
	registerBoolSetting("TargetESP", &targetEsp, targetEsp);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("Eggs", &eggs, eggs);
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

void Breaker::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())
		return;

	if (targetEsp) {
		if (bedsRender) {
			DrawUtils::setColor(133 / 255.0f, 16 / 255.0f, 14 / 255.0f, 1);
			DrawUtils::drawBox(renderPos.toVec3t(), vec3_t(renderPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (eggsRender) {
			DrawUtils::setColor(45 / 255.0f, 1 / 255.0f, 51 / 255.0f, 1);
			DrawUtils::drawBox(renderPos.toVec3t(), vec3_t(renderPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (cakesRender) {
			DrawUtils::setColor(199 / 255.0f, 97 / 255.0f, 36 / 255.0f, 1);
			DrawUtils::drawBox(renderPos.toVec3t(), vec3_t(renderPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (chestsRender) {
			DrawUtils::setColor(164 / 255.0f, 114 / 255.0f, 39 / 255.0f, 1);
			DrawUtils::drawBox(renderPos.toVec3t(), vec3_t(renderPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (barrelsRender) {
			DrawUtils::setColor(85 / 255.0f, 58 / 255.0f, 31 / 255.0f, 1);
			DrawUtils::drawBox(renderPos.toVec3t(), vec3_t(renderPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (redStoneRender) {
			DrawUtils::setColor(255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1);
			DrawUtils::drawBox(renderPos.toVec3t(), vec3_t(renderPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (shouldRenderEntity) {
			if (target != nullptr) {
				DrawUtils::drawEntityBox(target, (float)fmax(thick, 1 / (float)fmax(1, g_Data.getLocalPlayer()->getPos()->dist(*target->getPos()))));
			}
		}
	}
}

void Breaker::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	bool destroy = false;
	bool eat = false;

	bedsRender = false;
	eggsRender = false;
	cakesRender = false;
	chestsRender = false;
	barrelsRender = false;
	redStoneRender = false;
	shouldRotation = false;
	shouldRenderEntity = false;

	blockList.clear();

	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if ((id == 26 && beds) ||
					(id == 122 && eggs) ||
					(id == 92 && cakes) ||
					(id == 54 && chests) ||
					(id == 458 && barrels) ||
					((id == 73 || id == 74) && redStone)) {
					blockList.push_back(blockPos);
				}
			}
		}
	}

	++tick;
	if (!blockList.empty()) {
		std::sort(blockList.begin(), blockList.end(), [](const vec3_ti lhs, const vec3_ti rhs) {
			vec3_t localPos = *g_Data.getLocalPlayer()->getPos();
			return localPos.dist(lhs.toFloatVector()) < localPos.dist(rhs.toFloatVector());
			}); //距离优先

		//for (vec3_ti i : blockList) {
		int id = gm->player->region->getBlock(blockList[0])->toLegacy()->blockId;

		if (id == 26 && beds) {
			destroy = true;
			bedsRender = true;
			shouldRotation = true;
		} // Beds
		if (id == 122 && eggs) {
			destroy = true;
			eggsRender = true;
		} // Dragon Eggs
		if (id == 92 && cakes) {
			eat = true;
			cakesRender = true;
			shouldRotation = true;
		} // Cakes
		if (id == 54 && chests) {
			destroy = true;
			chestsRender = true;
		} // Chests
		if (id == 458 && barrels) {
			destroy = true;
			barrelsRender = true;
			shouldRotation = true;
		} // Barrels
		if ((id == 73 || id == 74) && redStone) {
			destroy = true;
			redStoneRender = true;
			selectPickaxe();
			shouldRotation = true;
		} // redStone

		if (rotations && shouldRotation) {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockList[0].toVec3t());
		}
		if (bedsRender || eggsRender || cakesRender || chestsRender || barrelsRender || redStoneRender) {
			renderPos = blockList[0];
		}

		if (tick >= delay) {
			if (destroy) {
				bool isDestroyedOut = false;
				gm->startDestroyBlock(blockList[0], 0, isDestroyedOut);
				gm->destroyBlock(&blockList[0], 0);
				gm->stopDestroyBlock(blockList[0]);
			}

			if (eat) {
				gm->buildBlock(&blockList[0], 0, true);
			}
		}

		//}
		/*
		if (rotations) {
			if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
				C_MovePlayerPacket packet(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);  //不动的时候Packet转头也能工作
			}
		}
		*/
	}


	g_Data.forEachEntity([this](C_Entity* ent, bool b) {
		std::string name = ent->getNameTag()->getText();

		if (g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
			if ((ent->getEntityTypeId() == 256 && (ent->height > 0.79f && ent->height < 0.81f) && (ent->width > 0.79f && ent->width < 0.81f) && treasures) ||
				(name.find("'s Bed") != std::string::npos && lifeboatBeds) ||
				((ent->height > 1.24f && ent->height < 1.26f) && (ent->width > 0.3 && ent->width < 0.5) && core)/* ||
				(name.find("Core") != std::string::npos && core)*/) { //Core上面的字是另一个实体的

				if (rotations) {
					angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*ent->getPos());
					shouldRotation = true;
				}
				target = ent;
				shouldRenderEntity = true;
				if (tick >= delay) {
					g_Data.getCGameMode()->attack(ent);
					//if (!moduleMgr->getModule<NoSwing>()->isEnabled())
					g_Data.getLocalPlayer()->swingArm();
				}
			}
		}
		});

	if (tick >= delay) {
		tick = 0;
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

void Breaker::selectPickaxe() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inventory = supplies->inventory;

	for (int n = 0; n < 36; n++) {
		C_ItemStack* stack = inventory->getItemStack(n);
		if (stack->item != nullptr && stack->getItem()->isPickaxe()) {
			supplies->selectedHotbarSlot = n;
			return;
		}
	}
}
