#pragma once

#include "Module.h"
#include "../../../Utils/Target.h"
#include "../../../Utils/TimerUtil.h"

class TriggerBot : public IModule {
private:
	int CPS = 20;
	TimerUtil CPSTime;

public:
	TriggerBot();
	~TriggerBot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
