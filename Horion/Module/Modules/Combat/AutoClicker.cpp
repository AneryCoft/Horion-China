#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "Automatically clicks for you.") {
	registerIntSetting("MaxCPS", &maxCPS, maxCPS, 1, 20);
	registerIntSetting("MinCPS", &minCPS, minCPS, 1, 20);
	registerBoolSetting("LeftClick", &leftClick, leftClick);
	registerBoolSetting("RightClick", &rightClick, rightClick);
	registerBoolSetting("Hold", &hold, hold);
	registerBoolSetting("Only Weapons", &weapons, weapons);
	//registerBoolSetting("Break Blocks", &breakBlocks, breakBlocks);
	registerBoolSetting("Use Item", &useItem, useItem);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");

}

void AutoClicker::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys())
		return;

	if (maxCPS < minCPS)
		minCPS = maxCPS;

	float CPS = randomFloat(maxCPS, minCPS);

	if (CPSTime.hasTimedElapsed(1000.f / CPS, true)) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		Level* level = g_Data.getLocalPlayer()->level;
		C_ItemStack* selectedItem = localPlayer->getSelectedItem();

		if (leftClick && (GameData::isLeftClickDown() || !hold)) {
			if (weapons && !selectedItem->isWeapon())
				return;

			g_Data.leftclickCount++;

			localPlayer->swingArm();

			if (level->hasEntity()) {
				gm->attack(level->getEntity());
			}
			else if (localPlayer->region->getBlock(level->block)->toLegacy()->blockId == 0) {
				LevelSoundEventPacket packet;
				packet.pos = *localPlayer->getPos();
				packet.sound = 42;
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
			}

			/*
			if (breakBlocks) {
				bool isDestroyed = false;
				gm->startDestroyBlock(level->block, level->blockSide, isDestroyed);
				gm->stopDestroyBlock(level->block);
				//if (isDestroyed && localPlayer->region->getBlock(level->block)->blockLegacy->blockId != 0)
				//	gm->destroyBlock(&level->block, 0);
			}*/
		}

		if (rightClick && (GameData::isRightClickDown() || !hold)) {
			g_Data.rightclickCount++;

			if (selectedItem->item != nullptr && (*selectedItem->item)->isBlock()) {
				if (level->rayHitType == 0) {
					gm->buildBlock(new vec3_ti(level->block), level->blockSide, true);
				}
			}
			else {
				if (useItem) {
					gm->useItem(*selectedItem);
				}
			}
		}
	}
}
