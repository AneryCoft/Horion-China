#include "Swing.h"

void* SmoothSwing = (void*)FindSignature("0F 84 ? ? ? ? 48 8B 56 ? 48 85 D2 74 ? 48 8B 02");

Swing::Swing() : IModule(0, Category::PLAYER, "Change swing") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Packet", 1))
		.addEntry(EnumEntry("Smooth", 2));
	registerEnumSetting("Mode", &mode, 2);
	registerBoolSetting("FakeBlock", &fakeBlock, fakeBlock);
}

const char* Swing::getModuleName() {
	return ("Swing");
}

void Swing::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	/*if (player == nullptr)
		return;*/

	auto hotbarStack = player->getSupplies()->inventory->getItemStack(player->getSupplies()->selectedHotbarSlot);
	shouldBlock = hotbarStack != nullptr && hotbarStack->item != nullptr && hotbarStack->getItem()->isSword() && g_Data.isRightClickDown();
	isAttacking = g_Data.isLeftClickDown();

	if (mode.selected == 2) {
		if (hotbarStack->item != nullptr && (*hotbarStack->item)->isFood() && (*hotbarStack->item)->itemId != 426 && (*hotbarStack->item)->itemId != 561/*ҩˮ*/) {
			Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
		}
		else {
			Utils::nopBytes((unsigned char*)SmoothSwing, 6);
		}
	}
}

void Swing::onDisable() {
	Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
}