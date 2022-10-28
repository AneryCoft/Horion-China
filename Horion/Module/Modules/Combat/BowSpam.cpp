#include "BowSpam.h"

BowSpam::BowSpam() : IModule(0, Category::COMBAT, "Shoot arrows like crazy") {
	registerIntSetting("Delay", &delay, delay, 4, 20);
}

BowSpam::~BowSpam() {
}

const char* BowSpam::getModuleName() {
	return ("BowSpam");
}

void BowSpam::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (!g_Data.canUseMoveKeys())
		return;

	if (localPlayer->getSelectedItemId() == 300/*¹­*/) {
		++tick;
		if (tick >= delay) {
			g_Data.getCGameMode()->releaseUsingItem();
			tick = 0;
		}
		else {
			gm->useItem(*localPlayer->getSelectedItem());
		}
	}
}