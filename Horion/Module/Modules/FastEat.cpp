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
	if (g_Data.getLocalPlayer() == nullptr && !g_Data.isInGame())
		return;
	else
		items.clear();//重进服citem指针会变

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);

		if (stack->item != nullptr && 
			(*stack->item)->itemId != 300 && //弓
			(*stack->item)->itemId != 575 && //弩
			(*stack->item)->getMaxUseDuration(stack) > duration/*防止某些服务器的资源包修改食物的MaxUseDuration*/) {
			if (items.find(*stack->item) == items.end()) {
				items[*stack->item] = (*stack->item)->getMaxUseDuration(stack); //保存修改前的MaxUseDuration
			}
			(*stack->item)->setMaxUseDuration(duration);
		}
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	for (auto& i : items) {
		i.first->setMaxUseDuration(i.second);
	}
	items.clear();
}
