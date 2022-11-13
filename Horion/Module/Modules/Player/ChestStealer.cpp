#include "ChestStealer.h"

ChestStealer::ChestStealer() : IModule(0, Category::PLAYER, "Automatically takes all items out of a chest.") {
	registerFloatSetting("Delay", &delay, delay, 0.f, 1000.f);
	registerFloatSetting("Open Delay", &openDelay, openDelay, 0.f, 1000.f);
	registerBoolSetting("Only Useful", &enhanced, enhanced);
	registerBoolSetting("No Delay", &noDelay, noDelay);
}

ChestStealer::~ChestStealer() {
}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

void ChestStealer::chestScreenController_tick(C_ChestScreenController* c) {
	if (c != nullptr) {
		if (canReset && !noDelay) {
			if (g_Data.getLocalPlayer()->canOpenContainerScreen())
				return;
			openDelayTime.resetTime();
			canReset = false;
		}
		if (openDelayTime.hasTimedElapsed(openDelay, false)) {
			std::vector<int> items = {};
			static auto invcleanerMod = moduleMgr->getModule<InventoryCleaner>();
			for (int i = 0; i < 54; i++) {
				C_ItemStack* stack = c->_getItemStack(TextHolder("container_items"), i);
				if (stack != nullptr && stack->item != NULL)
					if (!enhanced || invcleanerMod->stackIsUseful(stack))
						items.push_back(i);
			}
			if (!items.empty()) {
				if (noDelay) {
					for (int i : items) {
						c->handleAutoPlace(0x7FFFFFFF, "container_items", i);
					}
					c->tryExit();
					canReset = true;
					return;
				}
				else {
					if (delayTime.hasTimedElapsed(delay, true)) {
						c->handleAutoPlace(0x7FFFFFFF, "container_items", items[0]);
					}
				}
			}
			else {
				c->tryExit();
				canReset = true;
			}
		}
	}
}
