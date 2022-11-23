#include "AutoTool.h"

AutoTool::AutoTool() : IModule(0, Category::PLAYER, "Automatically select the fastest tool when digging blocks") {
}

AutoTool::~AutoTool() {
}

const char* AutoTool::getModuleName() {
	return ("AutoTool");
}

void AutoTool::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys() || !GameData::isLeftClickDown())
		return;

	Level* level = g_Data.getLocalPlayer()->level;
	if (level->rayHitType != 0)
		return; //是否有指向方块

	C_Block* pointingBlock = g_Data.getLocalPlayer()->region->getBlock(level->block);

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inventory = supplies->inventory;

	int8_t destroySpeed = 1;
	int8_t slot = -1;

	for (int8_t i = 0; i < 9; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr && (*stack->item)->isTool()) {
			int8_t currentDestroySpeed = (*stack->item)->getDestroySpeed(*stack, *pointingBlock);
			//logF("DestroySpeed = %i slot = %i", currentDestroySpeed, i);
			if (currentDestroySpeed > destroySpeed) {
				destroySpeed = currentDestroySpeed;
				slot = i;
			}
		}
	}
	if (slot != -1)
		supplies->selectedHotbarSlot = slot;
}
