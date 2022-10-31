#include "ChestAura.h"

ChestAura::ChestAura() : IModule(0, Category::PLAYER, "Aura but for opening chests.") {
	registerIntSetting("Range", &range, range, 1, 10);
	registerFloatSetting("Delay", &delay, delay, 0.f, 1000.f);
	registerBoolSetting("EnderChests", &enderchests, enderchests);
}

ChestAura::~ChestAura() {
}

const char* ChestAura::getModuleName() {
	return ("ChestAura");
}

void ChestAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer()->getSupplies()->inventory->isFull())
		return;

	if (!g_Data.getLocalPlayer()->canOpenContainerScreen())
		return;

	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti pos = vec3_ti(x, y, z);
				C_Block* block = gm->player->region->getBlock(pos);
				if (block != nullptr && g_Data.canUseMoveKeys()) {
					auto id = gm->player->region->getBlock(pos)->toLegacy()->blockId;
					bool open = false;
					if (id == 54) open = true;                  // Chests
					if (id == 130 && enderchests) open = true;  // EnderCheats
					if (open) {
						if (!(std::find(chestlist.begin(), chestlist.end(), pos) != chestlist.end())) {
							if (delayTime.hasTimedElapsed(delay, true)) {
								gm->buildBlock(&pos, 1, true);
								chestlist.push_back(pos);
								return;
							}
						}
					}
				}
			}
		}
	}
}

void ChestAura::onDisable() {
	chestlist.clear();  // this code should be changed later, the chestlist has to be cleared when loading into a new world
}
