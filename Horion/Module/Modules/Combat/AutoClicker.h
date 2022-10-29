#pragma once

#include "../Module.h"
#include "../../../../Utils/TimerUtil.h"

class AutoClicker : public IModule {
private:
	TimerUtil CPSTime;
	int maxCPS = 20;
	int minCPS = 20;
	bool leftClick = true;
	bool rightClick = true;
	bool weapons = false;
	bool hold = true;
	//bool breakBlocks = false;
	bool useItem = true;

public:
	AutoClicker();
	~AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
