#pragma once
#include "Module.h"
class FastEat : public IModule {
private:
	std::vector<C_Item*> items;
	//C_Item* food[];
public:
	int duration = 5;
	FastEat();
	~FastEat();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
