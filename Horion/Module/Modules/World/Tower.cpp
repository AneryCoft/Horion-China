#include "Tower.h"

Tower::Tower() : IModule(0, Category::WORLD, "Like scaffold, but vertically and a lot faster.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Motion", 0))
		.addEntry(EnumEntry("Timer", 1))
		.addEntry(EnumEntry("Jump", 2))
		.addEntry(EnumEntry("Teleport", 3));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("Motion", &motion, motion, 0.3f, 1.f);
	registerFloatSetting("Timer", &timer, timer, 20.f, 100.f);
	registerBoolSetting("AutoBlocks", &autoBlock, autoBlock);
	registerBoolSetting("RenderBlocks", &renderBlocks, renderBlocks);
	registerBoolSetting("Rotations", &rotations, rotations);
}

Tower::~Tower() {
}

const char* Tower::getModuleName() {
	return ("Tower");
}

void Tower::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
}

void Tower::onTick(C_GameMode* gm) {
	auto localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	static auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	auto selectedItem = localPlayer->getSelectedItem();
	if (!selectedItem->isValid() || !(*selectedItem->item)->isBlock()) {
		if (scaffoldMod->calcCount() == 0)
			return;

		if (autoBlock) {
			scaffoldMod->selectedBlock();
		}
		else {
			return;
		}
	} //手中是否有方块

	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return;

	vec3_t blockBelow = *localPlayer->getPos();
	blockBelow.y -= localPlayer->height;
	blockBelow.y -= 0.5f;
	//玩家脚下的方块

	blockBelow = blockBelow.floor();

	C_BlockLegacy* blockLegacy = localPlayer->region->getBlock(vec3_ti(blockBelow))->blockLegacy;

	if (blockLegacy->material->isReplaceable) {
		vec3_ti block(blockBelow);
		block = block.sub(vec3_ti(0, 1, 0));
		C_BlockLegacy* blockLegacy = localPlayer->region->getBlock(block)->blockLegacy;
		if (!blockLegacy->material->isReplaceable) {
			if (GameData::isKeyDown(*input->spaceBarKey)) {
				needRotations = true;

				switch (mode.selected) {
				case 0:
				{
					vec3_t moveVec;
					moveVec.x = localPlayer->velocity.x;
					moveVec.y = motion;
					moveVec.z = localPlayer->velocity.z;
					g_Data.getLocalPlayer()->lerpMotion(moveVec);
				}
				break;
				case 1:
					g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
					break;
				case 2:
					localPlayer->jumpFromGround();
					break;
				case 3:
					localPlayer->setPos(localPlayer->getPos()->add(0, 1, 0));
				}

				g_Data.getCGameMode()->buildBlock(&block, 1, true);
			}
			else {
				needRotations = false;
			}
		}
	}
}

void Tower::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (renderBlocks) {
		vec3_t blockBelow = *g_Data.getLocalPlayer()->getPos();
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;

		blockBelow = blockBelow.floor();

		DrawUtils::setColor(0.f, 0.3705f, 1.f, 1.f);
		DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(1), 0.4f);
	}
}

void Tower::onPlayerTick(C_Player* player) {
	if (rotations && needRotations) {
		player->pitch = 89.f;
	}
}

void Tower::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (rotations && needRotations) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = 89.f;
		}
	}
}