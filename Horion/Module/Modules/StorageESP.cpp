#include "StorageESP.h"

#include "../../DrawUtils.h"

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
	if (!g_Data.isInGame() || !GameData::canUseMoveKeys() || g_Data.getLocalPlayer() == nullptr)
		return;

	auto ourListLock = std::scoped_lock(this->listLock);

	for (const auto& chest : bufferedChestList) {
		auto storageID = g_Data.getLocalPlayer()->region->getBlock(chest.upper)->blockLegacy->blockId;
		float math = (float)fmax(0.3f, (float)fmin(1.f, 15));
		DrawUtils::setColor(1.f, 1.f, 1.f, math);

		vec3_t blockPos = chest.lower;
		if (blockPos.x < 0)
			blockPos.x -= 1;
		if (blockPos.z < 0)
			blockPos.z -= 1;
		storageID = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;

		auto mathVect = vec3_t(chest.upper.floor().add(vec3_t(1.f, 1.f, 1.f)).sub(chest.upper));
		mathVect.y = floor(mathVect.y);

		if (storageID == 54 && normalChest)
			DrawUtils::setColor(1.f, 1.f, 1.f, math);                     //Ïä×Ó
		else if (storageID == 146 && trappedChest)
			DrawUtils::setColor(.92f, .14f, .14f, math);                 //ÏÝÚåÏä
		else if (storageID == 130 && enderChest)
			DrawUtils::setColor(0.435294f, 0.215686f, 0.631372f, math);  //Ä©Ó°Ïä
		else if (storageID == 458 && barrel)
			DrawUtils::setColor(0.62f, 0.31f, 0.04f, math);                 //Í°
		else if (storageID == 205 && undyedShulkerBox)
			DrawUtils::setColor(.49f, .17f, .95f, math);                 //Î´È¾É«µÄÇ±Ó°ºÐ
		else if (storageID == 218 && shulkerBox)
			DrawUtils::setColor(.08f, .91f, .99f, math);                 //Ç±Ó°ºÐ
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