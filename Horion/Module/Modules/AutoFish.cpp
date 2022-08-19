#include "AutoFish.h"
//#include "../../../Utils/Logger.h"

AutoFish::AutoFish() : IModule(0, Category::PLAYER, "Automatically catches fish when using a rod") {
}

AutoFish::~AutoFish() {
}

const char* AutoFish::getModuleName() {
	return ("AutoFish");
}

static std::vector<C_Entity*> fishingHook;

void findFishingHook(C_Entity* entity, bool isRegularEntity) {
	if (entity->getEntityTypeId() == 77)
		fishingHook.push_back(entity);
}

void AutoFish::onTick(C_GameMode* gm) {
	C_ItemStack* item = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
	if ((*item->item)->itemId != 392)
		return; //�ж������Ƿ������

	fishingHook.clear();

	g_Data.forEachEntity(findFishingHook);

	if (startDelay)
		++delay;

	if (delay == 5) { //��ʱ0.25s
		gm->useItem(*item); //�ٴηų��㹳
		delay = 0;
		startDelay = false;
	}

	if (fishingHook.empty())
		return;

	//logF("\n%f\n%f\n%f", fishingHook[0]->velocity.x, fishingHook[0]->velocity.y, fishingHook[0]->velocity.z);

	if (fishingHook[0]->velocity.x == 0.f &&
		fishingHook[0]->velocity.y < -0.4f &&
		fishingHook[0]->velocity.y > -0.6f && //-0.496594,-0.499711,-0.496522,-0.500461
		fishingHook[0]->velocity.z == 0.f) {
		gm->useItem(*item);
		startDelay = true;
	}
}