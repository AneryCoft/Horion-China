#include "FluxSwing.h"
#include "../../../Memory/Hooks.h"

void* SmoothSwing = (void*)FindSignature("0F 84 ? ? ? ? 48 8B 56 ? 48 85 D2 74 ? 48 8B 02");

using namespace std;
FluxSwing::FluxSwing() : IModule(0, Category::VISUAL, "Swing like Flux and Fakeblock!") {
	registerBoolSetting("FakeBlock", &fkblock, fkblock);
	registerBoolSetting("ResetRotate", &reset, reset);
	registerFloatSetting("Rotate-X", &xPos, xPos, -3.f, 3.f);
	registerFloatSetting("Rotate-Y", &yPos, yPos, -3.f, 3.f);
	registerFloatSetting("Rotate-Z", &zPos, zPos, -3.f, 3.f);
}

const char* FluxSwing::getModuleName() {
	return ("FluxSwing");
}

void FluxSwing::onEnable() {
}

void FluxSwing::onTick(C_GameMode* gm) {
	auto killaura = moduleMgr->getModule<Killaura>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto slot = player->getSupplies()->inventory->getItemStack(player->getSupplies()->selectedHotbarSlot);
	shouldBlock = slot != nullptr && slot->item != nullptr && slot->getItem()->isWeapon() && g_Data.isRightClickDown() || slot != nullptr && slot->item != nullptr && slot->getItem()->isWeapon() && (killaura->isEnabled());
	isAttacking = g_Data.isLeftClickDown() || killaura->isEnabled();

	if (isEnabled()) Utils::nopBytes((unsigned char*)SmoothSwing, 6);
	else Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);

	if (reset) { 
		xPos = 0.f; yPos = 0.f; zPos = 0.f; 
		reset = false;
	}
}

void FluxSwing::onDisable() {
	Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
}