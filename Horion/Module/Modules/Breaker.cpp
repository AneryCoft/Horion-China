#include "Breaker.h"
#include "../../../Memory/Hooks.h"

Breaker::Breaker() : IModule(VK_NUMPAD9, Category::MISC, "Destroys certain blocks around you.") {
	registerIntSetting("Range", &range, range, 3, 10);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	this->registerFloatSetting("lineWidth", &this->thick, this->thick, 0.1f, 0.8f);
	registerBoolSetting("TargetESP", &this->targetEsp, this->targetEsp);
	registerBoolSetting("Rotations", &this->rotations, this->rotations);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("Eggs", &eggs, eggs);
	registerBoolSetting("Cakes", &cakes, cakes);
	registerBoolSetting("Chests", &chests, chests);
	registerBoolSetting("Barrels", &barrels, barrels);
	registerBoolSetting("RedStone", &redStone, redStone);
	registerBoolSetting("Treasures", &this->treasures, this->treasures);       //用于The Hive的Treasure Wars
	registerBoolSetting("Bed Item", &this->lifeboatBeds, this->lifeboatBeds);  //Lifeboat的实体床
	registerBoolSetting("Core", &this->core, this->core);                      //用于Galaxite的Core Wars
}

Breaker::~Breaker() {
}

const char* Breaker::getModuleName() {
	return ("Breaker");
}

void Breaker::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
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
		if (shouldRotation) {
			if (targetEsp)
				DrawUtils::drawEntityBox(target, (float)fmax(thick, 1 / (float)fmax(1, g_Data.getLocalPlayer()->getPos()->dist(*target->getPos()))));
		}
	}
}

static std::vector<vec3_ti> blockList;
void Breaker::onTick(C_GameMode* gm) {
	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	++tick;
	if (tick < delay) {
		return;
	}
	else {
		tick = 0;
	}

	blockList.clear();

	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if ((id == 26 && beds) ||
					(id == 122 && eggs) ||
					(id == 54 && chests) ||
					(id == 458 && barrels) ||
					((id == 73 || id == 74) && redStone)) {
					blockList.push_back(blockPos);
				}
			}
		}
	}

	bool destroy = false;
	bool eat = false;

	bedsRender = false;
	eggsRender = false;
	cakesRender = false;
	chestsRender = false;
	barrelsRender = false;
	redStoneRender = false;
	shouldRotation = false;

	if (!blockList.empty()) {
		//for (vec3_ti i : blockList) {
		int id = gm->player->region->getBlock(blockList[0])->toLegacy()->blockId;

		if (id == 26 && beds) {
			destroy = true;
			bedsRender = true;
		} // Beds
		if (id == 122 && eggs) {
			destroy = true;
			eggsRender = true;
		} // Dragon Eggs
		if (id == 92 && cakes) {
			eat = true;
			cakesRender = true;
		} // Cakes
		if (id == 54 && chests) {
			destroy = true;
			chestsRender = true;
		} // Chests
		if (id == 458 && barrels) {
			destroy = true;
			barrelsRender = true;
		} // Barrels
		if ((id == 73 || id == 74) && redStone) {
			destroy = true;
			redStoneRender = true;
			selectPickaxe();
		} // redStone

		if (destroy) {
			renderPos = blockList[0];
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockList[0].toVec3t());

			bool isDestroyedOut = false;
			gm->startDestroyBlock(blockList[0], 0, isDestroyedOut);
			gm->destroyBlock(&blockList[0], 0);
			//g_Data.getLocalPlayer()->swingArm();
			//break;
		}

		if (eat) {
			renderPos = blockList[0];
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockList[0].toVec3t());

			bool idk = true;
			gm->buildBlock(&blockList[0], 0, idk);
			//g_Data.getLocalPlayer()->swingArm();
			//break;
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
			if ((name.find("Treasure") != std::string::npos && treasures) ||
				(name.find("'s Bed") != std::string::npos && lifeboatBeds) ||
				(name.find("Core") != std::string::npos && core)) {

				if (rotations) {
					angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*ent->getPos());
				}
				target = ent;
				shouldRotation = true;
				g_Data.getCGameMode()->attack(ent);
				if (!moduleMgr->getModule<NoSwing>()->isEnabled())
					g_Data.getLocalPlayer()->swingArm();
			}
		}
		});
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
