#include "AutoClicker.h"
#include <random>

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you. (Turn on Hold mode when both LeftClick and RightClick are disabled.)") {
	LRclick = SettingEnum(this)
				  .addEntry(EnumEntry("Hold", 0))
				  .addEntry(EnumEntry("Left click", 1))
				  .addEntry(EnumEntry("Right click", 2))
				  .addEntry(EnumEntry("Both", 3));
	registerEnumSetting("Mode", &LRclick, 0);
	registerIntSetting("Max CPS", &maxcps, maxcps, 1, 20);
	registerIntSetting("Min CPS", &mincps, mincps, 1, 20);
	// registerBoolSetting("cpspertick", &cpspertick, cpspertick);
	registerBoolSetting("Only Weapons", &weapons, weapons);
	registerBoolSetting("Not Break Blocks", &breakBlocks, breakBlocks);
	registerBoolSetting("Play Sounds", &playsound, playsound);
	registerBoolSetting("Fast Place", &fastplace, fastplace);
	registerBoolSetting("Use Item", &useitem, useitem);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {

	return ("AutoClicker");
	
}

static int __fastcall gcd_func(int x, int y) {
	while (x != y) {
		if (x > y)
			x = x - y;
		else
			y = y - x;
	}
	return x;
}

void AutoClicker::onTick(C_GameMode* gm) {
	if (oldcps != maxcps) {
		oldcps = maxcps;
		if (maxcps % 20 != 0) 
			lcm = maxcps % 20 / gcd_func(maxcps % 20, 20) * 20;
		R_Odelay = 0.f;
		L_Odelay = 0.f;
		clock_temp_R = 0;
		clock_temp_L = 0;
	}
	if (mincps > maxcps) 
		mincps = maxcps;

	if (GameData::canUseMoveKeys()) {
		if (!fastplace && ((GameData::isRightClickDown() && LRclick.GetSelectedEntry().GetValue() == 0) || LRclick.GetSelectedEntry().GetValue() == 2 || LRclick.GetSelectedEntry().GetValue() == 3)) {
			Level* level = g_Data.getLocalPlayer()->level;
			clock_temp_R++;

			if (R_Odelay < clock_temp_R) {
				g_Data.rightclickCount++;
				bool idk = true;
				if (level->rayHitType == 0)
					gm->buildBlock(&vec3_ti(level->block), level->blockSide, idk);

				R_Odelay += 20.f / float(maxcps == mincps ? maxcps : mincps + rand() % (maxcps - mincps + 1));
			}

			if (useitem)
				g_Data.getCGameMode()->useItem(*g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot));
		}

		if ((GameData::isLeftClickDown() && LRclick.GetSelectedEntry().GetValue() == 0) || LRclick.GetSelectedEntry().GetValue() == 1 || LRclick.GetSelectedEntry().GetValue() == 3) {
			C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
			Level* level = g_Data.getLocalPlayer()->level;
			clock_temp_L++;

			LevelSoundEventPacket sounds;
			sounds.pos = *g_Data.getLocalPlayer()->getPos();
			sounds.sound = 42;

			if (L_Odelay < clock_temp_L) {
				auto selectedItem = localPlayer->getSelectedItem();
				if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
					return;
				g_Data.leftclickCount++;

				localPlayer->swingArm();

				if (level->hasEntity()) {
					if (level->getEntity() != nullptr)
						gm->attack(level->getEntity());
				} else if (breakBlocks && level->rayHitType == 0) {
					bool isDestroyed = false;
					gm->startDestroyBlock(level->block, level->blockSide, isDestroyed);
					gm->stopDestroyBlock(level->block);
					if (isDestroyed && localPlayer->region->getBlock(level->block)->blockLegacy->blockId != 0)
						gm->destroyBlock(&level->block, 0);
				} else if (playsound) 
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&sounds);

				L_Odelay += 20.f / float(maxcps == mincps ? maxcps : mincps + rand() % (maxcps - mincps + 1));
			}
		}
	}

	if (clock_temp_R >= lcm) {
		clock_temp_R = 0;
		R_Odelay = 0.f;
	}
	if (clock_temp_L >= lcm ) {
		clock_temp_L = 0;
		L_Odelay = 0.f;
	}
}

void AutoClicker::onLevelRender() {
	if (GameData::canUseMoveKeys()) {
		if (fastplace && ((GameData::isRightClickDown() && LRclick.GetSelectedEntry().GetValue() == 0) || LRclick.GetSelectedEntry().GetValue() == 2 || LRclick.GetSelectedEntry().GetValue() == 3)) {
			Level* level = g_Data.getLocalPlayer()->level;

			g_Data.rightclickCount++;
			bool idk = true;
			if (level->rayHitType == 0)
				g_Data.getCGameMode()->buildBlock(&vec3_ti(level->block), level->blockSide, idk);

			if (useitem) {
				g_Data.getCGameMode()->useItem(*g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot));
			}
		}
	}
}

//void AutoClicker::onSendPacket(C_Packet* packet) {
//	if (!g_Data.getLocalPlayer())
//		return;
//	if (!playsound && packet->isInstanceOf<LevelSoundEventPacket>() && reinterpret_cast<LevelSoundEventPacket*>(packet)->sound == 43) 
//		reinterpret_cast<LevelSoundEventPacket*>(packet)->sound = 0;
//}

void AutoClicker::onEnable() {
	oldcps = 0;
	lcm = 0;
	clock_temp_R = 0;
	clock_temp_L = 0;
	R_Odelay = 0.f;
	L_Odelay = 0.f;
	lcm = maxcps / gcd_func(maxcps, 20) * 20;

	// ftemp1 = 20.f / float(maxcps) ;
}

void AutoClicker::onDisable() {
	oldcps = 0;
	lcm = 0;
	clock_temp_R = 0;
	clock_temp_L = 0;
	R_Odelay = 0.f;
	L_Odelay = 0.f;
}
