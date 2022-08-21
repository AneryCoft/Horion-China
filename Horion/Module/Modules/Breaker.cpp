#include "Breaker.h"
#include "../../../Memory/Hooks.h"

Breaker::Breaker() : IModule(VK_NUMPAD9, Category::MISC, "Destroys certain blocks around you.") {
	registerIntSetting("Range", &range, range, 3, 10);
	this->registerFloatSetting("ESP Thickness", &this->thick, this->thick, 0.1f, 0.8f);
	registerBoolSetting("TargetESP", &this->targetEsp, this->targetEsp);
	registerBoolSetting("Rotations", &this->rotations, this->rotations);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("Eggs", &eggs, eggs);
	registerBoolSetting("Cakes", &cakes, cakes);
	registerBoolSetting("Chests", &chests, chests);
	registerBoolSetting("Barrels", &barrels, barrels);
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
			DrawUtils::setColor(0.5803f, 0.0901f, 0.0784f, 1);
			DrawUtils::drawBox(blockPos.toVec3t(), vec3_t(blockPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (eggsRender) {
			DrawUtils::setColor(0.2078f, 0.f, 0.2313f, 1);
			DrawUtils::drawBox(blockPos.toVec3t(), vec3_t(blockPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (cakesRender) {
			DrawUtils::setColor(0.7882f, 0.3843f, 0.1450f, 1);
			DrawUtils::drawBox(blockPos.toVec3t(), vec3_t(blockPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (chestsRender) {
			DrawUtils::setColor(0.6823f, 0.4823f, 0.1765f, 1);
			DrawUtils::drawBox(blockPos.toVec3t(), vec3_t(blockPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (barrelsRender) {
			DrawUtils::setColor(0.5450f, 0.4039f, 0.2353f, 1);
			DrawUtils::drawBox(blockPos.toVec3t(), vec3_t(blockPos.toVec3t()).add(1), (float)thick / (float)1.f, false);
		}
		if (shouldRotation) {
			if (targetEsp)
				DrawUtils::drawEntityBox(target, (float)fmax(thick, 1 / (float)fmax(1, g_Data.getLocalPlayer()->getPos()->dist(*target->getPos()))));
		}
	}
}

void Breaker::onTick(C_GameMode* gm) {
	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				blockPos = vec3_ti(x, y, z);
				bool destroy = false;
				bool eat = false;

				bedsRender = false;
				eggsRender = false;
				cakesRender = false;
				chestsRender = false;
				barrelsRender = false;
				shouldRotation = false;

				auto id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

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

				if (destroy) {
					gm->destroyBlock(&blockPos, 0);
					//g_Data.getLocalPlayer()->swingArm();
					return;
				}

				if (eat) {
					bool idk = true;
					gm->buildBlock(&blockPos, 0, idk);
					//g_Data.getLocalPlayer()->swingArm();
					return;
				}
			}
		}
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
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->pitch = angle.x;
			authInputPacket->yawUnused = angle.y;
			authInputPacket->yaw = angle.y;
		}
	}
}
