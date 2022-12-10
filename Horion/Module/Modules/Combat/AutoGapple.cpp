#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::COMBAT, "Auto heal if you're at low health.") {
	item = SettingEnum(this)
		.addEntry(EnumEntry("GApple", 0))
		.addEntry(EnumEntry("Soup", 1))
		.addEntry(EnumEntry("Potion", 2))
		.addEntry(EnumEntry("Spell of Life", 3));
	registerEnumSetting("Item", &item, 0);
	registerFloatSetting("Health", &health, health, 1.f, maxHealth);
	registerFloatSetting("Delay", &delay, delay, 0.f, 2000.f);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoHeal");
}

bool AutoGapple::targetItem(C_ItemStack* itemStack) {
	int16_t itemId = (*itemStack->item)->itemId;
	uint16_t itemData = itemStack->extraData;
	CompoundTag* itemTag = itemStack->tag;

	switch (item.selected) {
	case 0:
		if (itemId == 258 || itemId == 259) //金苹果和附魔金苹果 
			return true;
		break;
	case 1:
		if (itemId == 260) //蘑菇煲 
			return true;
		break;
	case 2:
		if (itemId == 561 && (itemData == 21 || itemData == 22)) //喷溅治疗药水 
			return true;
		break;
	case 3:
		if (itemId == 521 && itemTag != nullptr) {
			std::stringstream build;
			itemTag->write(build);
			std::string tagStr = build.str().c_str();
			if (tagStr.find("Spell of Life") != std::string::npos) { //The Hive的回血附魔书
				return true;
			}
			/*
			static const char* tagStr = u8"{display:{Lore:[\"§7Apply mending and heal\",\"§7yourself\"],Name:\"§r§dSpell of Life§7[Use]\"}}";
			if (!strcmp(build.str().c_str(), tagStr)) { //The Hive的回血附魔书
				return true;
			}
			*/
		}
		break;
	}
	return false;
}

void AutoGapple::selectedItem() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inventory = supplies->inventory;
	int emptySlot = 0;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (i < 9) {
			if (stack->item != nullptr) {
				if (targetItem(stack)) {
					//clientMessageF("data=%i", stack->getItemData());
					if (supplies->selectedHotbarSlot != i) {
						prevSlot = supplies->selectedHotbarSlot;
						supplies->selectedHotbarSlot = i;
					}
					break;
				}
			}
			else {
				emptySlot = i;
			}
		}
		else {
			if (stack->item != nullptr && targetItem(stack)) {
				inventory->swapSlots(i, emptySlot);
				if (supplies->selectedHotbarSlot != i) {
					prevSlot = supplies->selectedHotbarSlot;
					supplies->selectedHotbarSlot = i;
				}
				break;
			}
		}
	}
}

void AutoGapple::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	maxHealth = localPlayer->getAttribute(&HealthAttribute())->maximumValue;
	float loaclPlayerHealth = localPlayer->getAttribute(&HealthAttribute())->currentValue;

	if (loaclPlayerHealth > health) {
		rotation = false;
		return;
	}

	selectedItem();

	C_ItemStack* selectedItemStack = localPlayer->getSelectedItem();
	if (selectedItemStack->item == nullptr)
		return;

	if (targetItem(selectedItemStack)) {
		if (delayTime.hasTimedElapsed(delay, false)) {
			if (item.selected == 0 || item.selected == 1) {
				++tick;
				int duration = (*selectedItemStack->item)->getMaxUseDuration(selectedItemStack);
				if (tick > duration) {
					tick = 0;
					delayTime.resetTime();
					localPlayer->getSupplies()->selectedHotbarSlot = prevSlot;
					return;
				}
				else {
					gm->useItem(*selectedItemStack);
				}
			}
			else {
				if (item.selected == 2) {
					if (localPlayer->velocity.magnitude() < 0.01f) {
						C_MovePlayerPacket packet(localPlayer, *localPlayer->getPos());
						packet.pitch = 89.f;
						packet.yaw = localPlayer->yaw;
						packet.headYaw = localPlayer->bodyYaw;
						g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet); //不动的时候转头也能工作 
					}
					rotation = true;
				}
				gm->useItem(*selectedItemStack);
				delayTime.resetTime();
			}
		}
	}
}

void AutoGapple::onPlayerTick(C_Player* player) {
	if (rotation) {
		player->pitch = 89.f;
	}
}

void AutoGapple::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (rotation) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = 89.f;
		}
		/*else if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->pitch = 89.f;
		}*/
	}
}
