#pragma once

#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/TimerUtil.h"

class InfiniteAura : public IModule {
private:
	int CPS = 5;
	TimerUtil CPSTime;
	float tpDistance = 5.f;
	SettingEnum mode;
public:
	float range = 100;
	InfiniteAura();
	~InfiniteAura();
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};
