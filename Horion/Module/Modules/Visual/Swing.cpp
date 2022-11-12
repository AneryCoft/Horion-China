#include "Swing.h"

void* SmoothSwing = (void*)FindSignature("0F 84 ? ? ? ? 48 8B 56 ? 48 85 D2 74 ? 48 8B 02");

Swing::Swing() : IModule(0, Category::VISUAL, "Change swing") {
	swingMode = SettingEnum(this)
		.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("None", 1))
		.addEntry(EnumEntry("Packet", 2))
		.addEntry(EnumEntry("Smooth", 3));
	registerEnumSetting("SwingMode", &swingMode, 3);
	blockMode = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Block1", 1))
		.addEntry(EnumEntry("Block2", 2));
	registerEnumSetting("FakeBlockMode", &blockMode, 1);
}

const char* Swing::getModuleName() {
	return ("Swing");
}

void Swing::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	/*if (player == nullptr)
		return;*/

	auto hotbarStack = player->getSupplies()->inventory->getItemStack(player->getSupplies()->selectedHotbarSlot);
	shouldBlock = hotbarStack->isValid() && hotbarStack->getItem()->isSword() && g_Data.isRightClickDown();

	if (swingMode.selected == 3 && (hotbarStack->isValid() && !(
		(*hotbarStack->item)->isFood() ||
		(*hotbarStack->item)->itemId == 426 || //药水
		(*hotbarStack->item)->itemId == 561 || //喷溅药水
		(*hotbarStack->item)->itemId == 562 || //滞留药水
		(*hotbarStack->item)->itemId == 360 ||
		(*hotbarStack->item)->itemId == 361 ||
		(*hotbarStack->item)->itemId == 362 ||
		(*hotbarStack->item)->itemId == 363 ||
		(*hotbarStack->item)->itemId == 364 ||
		(*hotbarStack->item)->itemId == 365 ||
		(*hotbarStack->item)->itemId == 366 ||
		(*hotbarStack->item)->itemId == 367 ||
		(*hotbarStack->item)->itemId == 368 ||
		(*hotbarStack->item)->itemId == 369 //各种桶
		))) {
		Utils::nopBytes((unsigned char*)SmoothSwing, 6);
	}
	else {
		Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
	}
}

void Swing::onDisable() {
	Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
}