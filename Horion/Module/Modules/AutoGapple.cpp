#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::COMBAT, "Auto eat gapples if you're at low health.") {
	registerIntSetting("Health", &health, 10, 1, 20);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

void AutoGapple::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	float loaclPlayerHealth = localPlayer->getAttribute(&HealthAttribute())->currentValue;
	if (loaclPlayerHealth < health) {
		C_PlayerInventoryProxy* supplies = localPlayer->getSupplies();
		C_Inventory* inventory = supplies->inventory;
		C_ItemStack* stack;

		for (int i = 0; i < 9; i++) {
			stack = inventory->getItemStack(i);
			if (stack != nullptr && (*stack->item)->itemId == 258) {
				supplies->selectedHotbarSlot = i;
				break;
			}
		}

		if (localPlayer->getSelectedItemId() == 258) {
			gm->useItem(*stack);
		}
	}
}
