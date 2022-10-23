#include "TriggerBot.h"

TriggerBot::TriggerBot() : IModule(0, Category::COMBAT, "Attacks entities you're looking at.") {
	registerIntSetting("CPS", &CPS, CPS, 1, 20);
}

TriggerBot::~TriggerBot() {
}

const char* TriggerBot::getModuleName() {
	return ("TriggerBot");
}

void TriggerBot::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_Entity* target = g_Data.getLocalPlayer()->level->getEntity();

	if (target != 0 && CPSTime.hasTimedElapsed(1000.f / (float)CPS, true)) {
		if (!Target::isValidTarget(target))
			return;

		localPlayer->swingArm();
		gm->attack(target);
	}
}