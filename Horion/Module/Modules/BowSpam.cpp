#include "BowSpam.h"

BowSpam::BowSpam() : IModule(0, Category::COMBAT, "Bow") {
	registerIntSetting("Delay", &delay, delay, 0, 20);
}

BowSpam::~BowSpam() {
}

const char* BowSpam::getModuleName() {
	return ("BowSpam");
}

void BowSpam::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (g_Data.isRightClickDown && localPlayer->getSelectedItemId() == 300/*¹­*/) {
		++tick;
		if (tick >= delay) {
			g_Data.getCGameMode()->releaseUsingItem();
			tick = 0;
		}
	}
}