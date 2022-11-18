#include "Extinguisher.h"

Extinguisher::Extinguisher() : IModule(0, Category::WORLD, "Extinguish the fire around you.") {
	registerIntSetting("Range", &range, range, 3, 10);
}

Extinguisher::~Extinguisher() {
}

const char* Extinguisher::getModuleName() {
	return ("Extinguisher");
}

void Extinguisher::onTick(C_GameMode* gm) {
	auto localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	vec3_t* localPlayerPos = localPlayer->getPos();

	for (int x = (int)localPlayerPos->x - range; x < localPlayerPos->x + range; x++) {
		for (int z = (int)localPlayerPos->z - range; z < localPlayerPos->z + range; z++) {
			for (int y = (int)localPlayerPos->y - range; y < localPlayerPos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				short blockId = localPlayer->region->getBlock(blockPos)->toLegacy()->blockId;

				if (blockId == 51) { //»ð
					vec3_ti actionPos = blockPos.add(0, -1, 0);
					bool isDestroyedOut = false;
					gm->startDestroyBlock(actionPos, 1, isDestroyedOut);
					gm->stopDestroyBlock(actionPos);
				}
			}
		}
	}
}