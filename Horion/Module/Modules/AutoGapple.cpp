#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::COMBAT, "Auto eat gapples if you're at low health.") {
	item = SettingEnum(this)
		.addEntry(EnumEntry("GApple", 0))
		.addEntry(EnumEntry("Soup", 1))
		.addEntry(EnumEntry("Potion", 2));
	registerEnumSetting("Mode", &item, 0);
	registerFloatSetting("Health", &health, this->health, 1.f, maxHealth);
	registerIntSetting("Delay", &delay, this->delay, 0, 20);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoHeal");
}

bool AutoGapple::targetItem(int16_t itemId, uint16_t extraData) {
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
		if (itemId == 561 && (extraData == 21 || extraData == 22)) //喷溅治疗药水 
			return true;
	}
	return false;
}

void AutoGapple::onTick(C_GameMode* gm) {
	rotation = false;
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	maxHealth = localPlayer->getAttribute(&HealthAttribute())->maximumValue;
	float loaclPlayerHealth = localPlayer->getAttribute(&HealthAttribute())->currentValue;

	if (loaclPlayerHealth < health) {
		/*
		switch (mode.selected) {
		case 0:
			itemID = 258 || 259; //金苹果和附魔金苹果 
			break;
		case 1:
			itemID = 260; //蘑菇煲 
			break;
		case 2:
			itemID = 561; //喷溅药水 
		}
		*/
		C_PlayerInventoryProxy* supplies = localPlayer->getSupplies();
		C_Inventory* inventory = supplies->inventory;
		C_ItemStack* stack;
		int emptySlot = 0;

		for (int i = 0; i < 36; i++) {
			stack = inventory->getItemStack(i);
			if (i < 9) {
				if (stack->item != nullptr) {
					if (targetItem((*stack->item)->itemId, stack->extraData)) {
						//clientMessageF("data=%i", stack->getItemData());
						supplies->selectedHotbarSlot = i;
						break;
					}
				}
				else {
					emptySlot = i;
				}
			}
			else {
				if (stack->item != nullptr && targetItem((*stack->item)->itemId, stack->extraData)) {
					inventory->moveItem(i, emptySlot);
					supplies->selectedHotbarSlot = emptySlot;
					break;
				}
			}
		}

		C_ItemStack* selectedItemStack = localPlayer->getSelectedItem();
		if (selectedItemStack->item == nullptr)
			return;

		int16_t selectedItemId = (*selectedItemStack->item)->itemId;
		uint16_t selectedItemData = selectedItemStack->extraData;

		if (targetItem(selectedItemId, selectedItemData)) {
			static auto fastEatMod = moduleMgr->getModule<FastEat>();
			++tick;
			if (item.selected == 1/*蘑菇煲在大多数服务器都是秒吃 */ || item.selected == 2 || candelay) {
				if (tick < delay) {
					return;
				}
				else {
					tick = 0;
					candelay = false;
				}
			}
			else {
				if (fastEatMod->isEnabled()) {
					duration = fastEatMod->duration;
				}
				else
				{
					duration = 32;
				}
				if (tick == duration) {
					tick = 0;
					candelay = true;
				}
			}

			if (item.selected == 2) {
				if (localPlayer->velocity.squaredxzlen() < 0.01) {
					C_MovePlayerPacket packet(localPlayer, *localPlayer->getPos());
					packet.pitch = 89.f;
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet); //不动的时候转头也能工作 
				}
				rotation = true;
			}

			gm->useItem(*stack);
		}
	}
}

void AutoGapple::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (rotation) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = 89.f;
		}
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->pitch = 89.f;
		}
	}
}
