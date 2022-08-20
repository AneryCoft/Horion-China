#include "FastEat.h"

FastEat::FastEat() : IModule(0, Category::PLAYER, "Eat food almost instant") {
	registerIntSetting("Duration", &this->duration, this->duration, 1, 32);
}

FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return ("FastEat");
}

void FastEat::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->getMaxUseDuration(stack) == 32) {
			(*stack->item)->setMaxUseDuration(duration);
			items.push_back(*stack->item);
		}
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	for (C_Item* i : items) { //这里为啥要刻意改auto
		i->setMaxUseDuration(32);
	}

	items.clear();
	/*
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->getMaxUseDuration(stack) == duration) {
			(*stack->item)->setMaxUseDuration(32);
		}
	}*/
}
