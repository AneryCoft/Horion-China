#include "FastEat.h"

FastEat::FastEat() : IModule(0, Category::PLAYER, "Eat food almost instant") {
	registerIntSetting("Duration", &this->duration, this->duration, 1, 31);
}

FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return ("FastEat");
}

void FastEat::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	if (g_Data.getLocalPlayer() != nullptr && g_Data.isInGame()) {
		for (int i = 0; i < 36; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nulllptr && (*stack->item)->itemId != 261 && (*stack->item)->getMaxUseDuration(stack) > 0/*防止某些服务器的资源包修改食物的MaxUseDuration*/) {
				if (items.find(*stack->item) == items.end()) {
					items[*stack->item] = (*stack->item)->getMaxUseDuration(stack);
				}
				(*stack->item)->setMaxUseDuration(duration);
			}
		}
	} else {
		items.clear();//重进服citem指针会变
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	for (auto& i : items) {
		__try {
			i.first->setMaxUseDuration(i.second);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			items.clear();
			return;
		}
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
