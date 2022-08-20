#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::COMBAT, "Auto eat gapples if you're at low health.") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("GApple", 0))
		.addEntry(EnumEntry("Soup", 1))
		.addEntry(EnumEntry("Potion", 2));
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("Health", &health, this->health, 1.f, 20.f);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoHeal");
}

void AutoGapple::onTick(C_GameMode* gm) {
	rotation = false;

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	float loaclPlayerHealth = localPlayer->getAttribute(&HealthAttribute())->currentValue;

	if (loaclPlayerHealth < health) {
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

		C_PlayerInventoryProxy* supplies = localPlayer->getSupplies();
		C_Inventory* inventory = supplies->inventory;
		C_ItemStack* stack;

		for (int i = 0; i < 9; i++) {
			stack = inventory->getItemStack(i);

			if (stack != nullptr && *stack->item != nullptr) {
				if ((*stack->item)->itemId == itemID) {
					if (mode.selected == 2) {
						//clientMessageF("data=%i", stack->getItemData());
						if (stack->extraData != 21 && stack->extraData != 22) //治疗药水
							continue;
					}
					supplies->selectedHotbarSlot = i;
					break;
				}
			}
		}

		int selectedItemId = localPlayer->getSelectedItemId();
		if (selectedItemId == itemID) {
			if (mode.selected == 2) {
				if (localPlayer->velocity.squaredxzlen() < 0.01) {
					C_MovePlayerPacket packet(localPlayer, *localPlayer->getPos());
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
			auto* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->pitch = 89.f;
		}
	}
}