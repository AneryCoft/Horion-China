#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/TimerUtil.h"

class ChestStealer : public IModule {
private:
	float delay = 0.f;
	TimerUtil delayTime;
	float openDelay = 0.f;
	TimerUtil openDelayTime;
	bool enhanced = true;
	bool noDelay = false; 
	bool canReset = true;

public:
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void chestScreenController_tick(C_ChestScreenController* c);
	virtual const char* getModuleName() override;
};
