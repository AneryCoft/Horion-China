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
	registerBoolSetting("AutoBlock", &autoBlock, autoBlock);
}

Swing::~Swing() {
}

const char* Swing::getModuleName() {
	return ("Swing");
}

void Swing::onTick(C_GameMode* gm) {
	auto localPlayer = g_Data.getLocalPlayer();
	/*if (player == nullptr)
		return;*/

	auto hotbarStack = localPlayer->getSelectedItem();
	static auto killauraMod = moduleMgr->getModule<Killaura>();
	shouldBlock = hotbarStack->isValid() && 
		hotbarStack->getItem()->isSword() && 
		(g_Data.isRightClickDown() || (autoBlock ? !killauraMod->targetListEmpty:false));

	if (swingMode.selected == 3) {
		if (hotbarStack->isValid()) {
			int16_t itemId = (*hotbarStack->item)->itemId;
			if (!(
					(*hotbarStack->item)->isFood() ||
					itemId == 426 || //药水
					itemId == 561 || //喷溅药水
					itemId == 562 || //滞留药水
					(itemId >= 360 && itemId <= 369) //各种桶
					)) {
				Utils::nopBytes((unsigned char*)SmoothSwing, 6);
				return;
			}
		}
		Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
	}
}

void Swing::onDisable() {
	Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
}