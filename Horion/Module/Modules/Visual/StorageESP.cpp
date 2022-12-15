#include "StorageESP.h"

StorageESP::StorageESP() : IModule(0, Category::VISUAL, "ESP for but storage blocks.") {
	registerBoolSetting("Normal Chest", &normalChest, normalChest);
	registerBoolSetting("Trapped Chest", &trappedChest, trappedChest);
	registerBoolSetting("Ender Chest", &enderChest, enderChest);
	registerBoolSetting("Barrel", &barrel, barrel);
	registerBoolSetting("Undyed Shulker Box", &undyedShulkerBox, undyedShulkerBox);
	registerBoolSetting("Shulker Box", &shulkerBox, shulkerBox);
}

StorageESP::~StorageESP() {
}

const char* StorageESP::getModuleName() {
	return ("StorageESP");
}

void StorageESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!GameData::canUseMoveKeys() || g_Data.getLocalPlayer() == nullptr)
		return;

	auto ourListLock = std::scoped_lock(this->listLock);

	for (const auto& chest : bufferedChestList) {
		auto storageID = g_Data.getLocalPlayer()->region->getBlock(chest.upper)->blockLegacy->blockId;
		float math = (float)fmax(0.3f, (float)fmin(1.f, 15));
		//DrawUtils::setColor(1.f, 1.f, 1.f, math);

		vec3_t blockPos = chest.lower;
		if (blockPos.x < 0)
			blockPos.x -= 1;
		if (blockPos.z < 0)
			blockPos.z -= 1;
		storageID = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;

		auto mathVect = vec3_t(chest.upper.floor().add(vec3_t(1.f, 1.f, 1.f)).sub(chest.upper));
		mathVect.y = floor(mathVect.y);

		if (storageID == 54 && normalChest)
			DrawUtils::setColor(164 / 255.f, 114 / 255.f, 39 / 255.f, math); //Ïä×Ó
		else if (storageID == 146 && trappedChest)
			DrawUtils::setColor(82 / 255.f, 7 / 255.f, 0 / 255.f, math); //ÏÝÚåÏä
		else if (storageID == 130 && enderChest)
			DrawUtils::setColor(36 / 255.f, 53 / 255.f, 55 / 255.f, math); //Ä©Ó°Ïä
		else if (storageID == 458 && barrel)
			DrawUtils::setColor(85 / 255.f, 58 / 255.f, 31 / 255.f, math); //Ä¾Í°
		else if (storageID == 205 && undyedShulkerBox)
			DrawUtils::setColor(151 / 255.f, 104 / 255.f, 151 / 255.f, math); //Î´È¾É«µÄÇ±Ó°ºÐ
		else if (storageID == 218 && shulkerBox)
			DrawUtils::setColor(57 / 255.f, 178 / 255.f, 218 / 255.f, math); //Ç±Ó°ºÐ
		else
			continue;

		DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, g_Data.getLocalPlayer()->eyePos0.dist(chest.lower))), true);  // Fancy math to give an illusion of good esp
	}
}

void StorageESP::onTick(C_GameMode* gm) {
	// Swap list
	auto listLock = g_Data.lockChestList();
	auto& chestList = g_Data.getChestList();
	auto ourListLock = std::scoped_lock(this->listLock);

	this->bufferedChestList.clear();
	this->bufferedChestList.insert(this->bufferedChestList.end(), chestList.begin(), chestList.end());
	chestList.clear();
}