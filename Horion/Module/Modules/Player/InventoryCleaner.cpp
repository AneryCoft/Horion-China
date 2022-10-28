#include "InventoryCleaner.h"

InventoryCleaner::InventoryCleaner() : IModule(0, Category::PLAYER, "Automatically throws not needed stuff out of your inventory.") {
	registerFloatSetting("Delay", &delay, delay, 0.f, 1000.f);
	registerBoolSetting("KeepWeapons", &keepWeapons, keepWeapons);
	registerBoolSetting("KeepTools", &keepTools, keepTools);
	registerBoolSetting("KeepArmor", &keepArmor, keepArmor);
	registerBoolSetting("KeepFood", &keepFood, keepFood);
	registerBoolSetting("KeepBlocks", &keepBlocks, keepBlocks);
	registerBoolSetting("OpenInv", &openInv, openInv);
	registerBoolSetting("AutoSort", &autoSort, autoSort);
}

InventoryCleaner::~InventoryCleaner() {
}

const char* InventoryCleaner::getModuleName() {
	return ("InvCleaner");
}

void InventoryCleaner::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer()->canOpenContainerScreen() && openInv)
		return;

	std::vector<int> dropSlots = findUselessItems();
	static int i = 0;
	if (i >= dropSlots.size())
		i = 0;

	if (!dropSlots.empty()) {
		if (dropTime.hasTimedElapsed(delay, true)) {
			g_Data.getLocalPlayer()->getSupplies()->inventory->dropSlot(dropSlots[i]);
			++i;
		}
		/*for (int i : dropSlots) {
			g_Data.getLocalPlayer()->getSupplies()->inventory->dropSlot(i);
		}*/
	} //����������Ʒ
	else {
		dropTime.resetTime();
		i = 0;
		if (autoSort) {
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* inv = supplies->inventory;
			float damage = 0;

			int weapon = 0;
			int pickaxe = 1;
			int axe = 2;
			int shovel = 3;
			int gApple = 4;

			for (int n = 0; n < 36; n++) {
				C_ItemStack* stack = inv->getItemStack(n);
				if (stack->item != NULL) {
					if ((*stack->item)->itemId == 258) { //��ƻ��
						gApple = n;
					}
					else if ((*stack->item)->isPickaxe()) { //��
						pickaxe = n;
					}
					else if ((*stack->item)->isAxe()) { //��
						axe = n;
					}
					else if ((*stack->item)->isShovel()) { //��
						shovel = n;
					}
					else {
						float currentDamage = stack->getAttackingDamageWithEnchants();
						if (currentDamage > damage) {
							damage = currentDamage;
							weapon = n;
						}
					} //����
				}
			}

			if (weapon != 0) inv->swapSlots(weapon, 0); //�ѽ��ŵ���һ�������
			if (pickaxe != 1) inv->swapSlots(pickaxe, 1); //�Ѹ�ŵ��ڶ��������
			if (axe != 2) inv->swapSlots(axe, 2); //�Ѹ��ŵ������������
			if (shovel != 3) inv->swapSlots(shovel, 3); //���·ŵ����ĸ������
			if (gApple != 4) inv->swapSlots(gApple, 4); //�ѽ�ƻ���ŵ�����������
		}
	}
}

std::vector<int> InventoryCleaner::findStackableItems() {
	std::vector<int> stackableSlot;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr) {
			if ((*itemStack->item)->getMaxStackSize(0) > itemStack->count) {
				for (int i2 = 0; i2 < 36; i2++) {
					if (i2 == i) continue;
					C_ItemStack* itemStack2 = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i2);
					if ((*itemStack2->item)->getMaxStackSize(0) > itemStack->count) {
						if (*itemStack->item == *itemStack2->item) {
							if ((std::find(stackableSlot.begin(), stackableSlot.end(), i2) == stackableSlot.end())) stackableSlot.push_back(i2);
							if ((std::find(stackableSlot.begin(), stackableSlot.end(), i) == stackableSlot.end())) stackableSlot.push_back(i);
						}
					}
				}
			}
		}
	}
	return stackableSlot;
} //�ѵ���Ʒ

std::vector<int> InventoryCleaner::findUselessItems() {
	// Filter by options

	std::vector<int> uselessItems;
	std::vector<C_ItemStack*> items;

	{
		for (int i = 0; i < 36; i++) {
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if (!stackIsUseful(itemStack)) {
					if (std::find(items.begin(), items.end(), itemStack) == items.end())
						uselessItems.push_back(i); //����������Ʒ
					else
						items.push_back(itemStack);
				}
				/*
				else if (std::find(items.begin(), items.end(), itemStack) == items.end()) {
					if ( !isLastItem(*itemStack->item))
						uselessItems.push_back(i); //��������һ�����õ���Ʒ
					else
						items.push_back(itemStack);
				}*/
			}
		}

		for (int i = 0; i < 4; i++) {
			if (g_Data.getLocalPlayer()->getArmor(i)->item != nullptr)
				items.push_back(g_Data.getLocalPlayer()->getArmor(i));
		}
	}

	if (items.empty())
		return uselessItems;

	//��������
	{
		int attackDamage = 1;

		for (int i = 0; i < 36; i++) {
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;

			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if (itemStack->isWeapon()) {
					//clientMessageF("Damage=%f", itemStack->getAttackingDamageWithEnchants());
					if (itemStack->getAttackingDamageWithEnchants() > attackDamage) { //�ȽϹ����˺�
						attackDamage = itemStack->getAttackingDamageWithEnchants();
					}
					else {
						if (!keepTools || !(*itemStack->item)->isTool()) //��������
							uselessItems.push_back(i);
					}
				}
			}
		}
	}

	/*
	std::sort(items.begin(), items.end(), [](const C_ItemStack* lhs, const C_ItemStack* rhs) {
		C_ItemStack* current = const_cast<C_ItemStack*>(lhs);
		C_ItemStack* other = const_cast<C_ItemStack*>(rhs);
		return current->getAttackingDamageWithEnchants() > other->getAttackingDamageWithEnchants();
		}); //ͨ���������˺�������

	bool hadTheBestItem = false;
	C_ItemStack* bestItem = items.at(0);

	for (int i = 0; i < 36; i++) {
		if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
			continue;
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr && itemStack->getAttackingDamageWithEnchants() > 1) {
			if (itemStack->getAttackingDamageWithEnchants() < bestItem->getAttackingDamageWithEnchants()) {
				if (!keepTools || !(*itemStack->item)->isTool()) { //��������
					uselessItems.push_back(i);
				}
			}
			else {
				//�˺������ֵ��ͬ
				if (hadTheBestItem)
					if (!keepTools || !(*itemStack->item)->isTool()) { //��������
						uselessItems.push_back(i);
					}
					else
						hadTheBestItem = true;
			}
		}
	}
	*/

	//���˹�
	{
		int enchantLevel = -1;

		for (int i = 0; i < 36; i++) {
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;

			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if ((*itemStack->item)->itemId == 300) {
					if (itemStack->getEnchantValue(19) > enchantLevel) { //�Ƚ��������Եĵȼ�
						enchantLevel = itemStack->getEnchantValue(19);
					}
					else {
						uselessItems.push_back(i);
					}
				}
			}
		}
	}

	//����װ��
	{
		std::vector<C_ItemStack*> helmets;
		std::vector<C_ItemStack*> chestplates;
		std::vector<C_ItemStack*> leggings;
		std::vector<C_ItemStack*> boots;

		//ͨ������ֵ����
		std::sort(items.begin(), items.end(), [](const C_ItemStack* lhs, const C_ItemStack* rhs) {
			C_ItemStack* current = const_cast<C_ItemStack*>(lhs);
			C_ItemStack* other = const_cast<C_ItemStack*>(rhs);
			return current->getArmorValueWithEnchants() > other->getArmorValueWithEnchants();
			});

		// Put armor items in their respective vectors
		for (C_ItemStack* itemsteck : items) {
			C_Item* item = itemsteck->getItem();
			if (item->isArmor()) {
				C_ArmorItem* armorItem = reinterpret_cast<C_ArmorItem*>(item);
				if (armorItem->isHelmet())
					helmets.push_back(itemsteck);
				else if (armorItem->isChestplate())
					chestplates.push_back(itemsteck);
				else if (armorItem->isLeggings())
					leggings.push_back(itemsteck);
				else if (armorItem->isBoots())
					boots.push_back(itemsteck);
			}
		}
		bool hadBest[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++) {
			C_ItemStack* itemsteck = g_Data.getLocalPlayer()->getArmor(i);
			C_Item** item = itemsteck->item;
			if (item != nullptr) {

				C_ArmorItem* armor = reinterpret_cast<C_ArmorItem*>(*item);

				float testArmorValue = 0;
				switch (armor->ArmorSlot) {
				case 0:
					if (helmets.size() > 0)
						testArmorValue = helmets.at(0)->getArmorValueWithEnchants();
					break;
				case 1:
					if (chestplates.size() > 0)
						testArmorValue = chestplates.at(0)->getArmorValueWithEnchants();
					break;
				case 2:
					if (leggings.size() > 0)
						testArmorValue = leggings.at(0)->getArmorValueWithEnchants();
					break;
				case 3:
					if (boots.size() > 0)
						testArmorValue = boots.at(0)->getArmorValueWithEnchants();
					break;
				}
				if (itemsteck->getArmorValueWithEnchants() >= testArmorValue)
					hadBest[armor->ArmorSlot] = true;
			}
		}

		for (int i = 0; i < 36; i++) {
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;  // item already useless
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr && (*itemStack->item)->isArmor()) {
				C_ArmorItem* armor = reinterpret_cast<C_ArmorItem*>(*itemStack->item);
				if (armor->isHelmet()) {
					if (hadBest[0] || itemStack->getArmorValueWithEnchants() < helmets.at(0)->getArmorValueWithEnchants()) {
						uselessItems.push_back(i);
					}
					else
						hadBest[0] = true;
				}
				else if (armor->isChestplate()) {
					if (hadBest[1] || itemStack->getArmorValueWithEnchants() < chestplates.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[1] = true;
				}
				else if (armor->isLeggings()) {
					if (hadBest[2] || itemStack->getArmorValueWithEnchants() < leggings.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[2] = true;
				}
				else if (armor->isBoots()) {
					if (hadBest[3] || itemStack->getArmorValueWithEnchants() < boots.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[3] = true;
				}
			}
		}
	}

	//���˹���
	//C_ItemStack* bestTool;
	int maxDamage = 0;
	int maxDamage1 = 0;
	int maxDamage2 = 0;

	for (int i = 0; i < 36; i++) {
		if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
			continue;

		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr/* && (*itemStack->item)->isTool()*/) {

			if ((*itemStack->item)->isPickaxe()) {
				if ((*itemStack->item)->getMaxDamage() > maxDamage) {
					maxDamage = (*itemStack->item)->getMaxDamage();
					//bestTool = itemStack;
				}
				else {
					uselessItems.push_back(i);
				}
			} //��

			else if ((*itemStack->item)->isAxe()) {
				if ((*itemStack->item)->getMaxDamage() > maxDamage1) {
					maxDamage1 = (*itemStack->item)->getMaxDamage();
					//bestTool = itemStack;
				}
				else {
					uselessItems.push_back(i);
				}
			} //��

			else if ((*itemStack->item)->isShovel()) {
				if ((*itemStack->item)->getMaxDamage() > maxDamage2) {
					maxDamage2 = (*itemStack->item)->getMaxDamage();
					//bestTool = itemStack;
				}
				else {
					uselessItems.push_back(i);
				}
			} //��

		}
	}

	return uselessItems;
}

bool InventoryCleaner::stackIsUseful(C_ItemStack* itemStack) { //��Ҫ��������Ʒ
	if (itemStack->item == nullptr) return true;
	if (keepWeapons && itemStack->isWeapon()) return true;    //����
	if (keepWeapons && (*itemStack->item)->isShooter()) return true;    //�� ��
	if (keepArmor && (*itemStack->item)->isArmor()) return true;      //����
	if (keepTools && (*itemStack->item)->isTool()) return true;       //����
	if (keepFood && (*itemStack->item)->isFood()) return true;        //ʳ��
	if (keepBlocks && (*itemStack->item)->isBlock()) return true;     //����
	//if (keepTools && (*itemStack->item)->itemId == 368) return true;  //ĩӰ����
	return false;
}

bool InventoryCleaner::isLastItem(C_Item* item) { //����Ҫ��������Ʒ���д���
	std::vector<C_Item*> items;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (stack->item != nullptr)
			items.push_back((*stack->item));
	}
	int count = 0;
	for (C_Item* a : items) {
		if (a == item)
			count++;
	}
	if (count > 1)
		return false; //ȥ���ظ���Ʒ

	return true;
}
