#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/TimerUtil.h"

class InventoryCleaner : public IModule {
private:
	bool sorted = false;

	std::vector<int> findStackableItems();
	std::vector<int> findUselessItems();
	bool isLastItem(C_Item* item);

	bool keepWeapons = true;
	bool keepTools = true;
	bool keepArmor = true;
	bool keepBlocks = true;
	bool keepFood = true;

	bool openInv = true;
	bool autoSort = false;
	float delay = 0.f;
	bool noDelay = false;

	TimerUtil dropTime;

public:
	bool stackIsUseful(C_ItemStack* itemStack);
	InventoryCleaner();
	~InventoryCleaner();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	//virtual void onGetPickRange() override;
};
