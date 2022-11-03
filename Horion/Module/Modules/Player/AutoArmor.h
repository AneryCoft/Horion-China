#pragma once

#include "../Module.h"

class AutoArmor : public IModule {
private:
	bool onlyInv = false;

public:
	C_CraftingScreenController* inventoryScreen = nullptr;
	AutoArmor();
	~AutoArmor();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
