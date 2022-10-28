#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/TimerUtil.h"

class HackerDetector : public IModule {
private:
	float VL = 0.f;
	TimerUtil flyTime;
	bool speed = true;
	bool fly = true;
	bool velocity = true;
public:
	HackerDetector();
	~HackerDetector();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
