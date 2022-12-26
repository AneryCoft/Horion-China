#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/TimerUtil.h"

class InfiniteAura : public IModule {
private:
	int CPS = 5;
	TimerUtil CPSTime;
	float tpDistance = 5.f;
	SettingEnum mode;
	//SettingEnum packetMode;
	std::vector<vec3_t> posList = {};
	bool renderPos = true;
public:
	std::vector<C_Entity*> targetList = {};
	float range = 100.f;

	InfiniteAura();
	~InfiniteAura();
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
};
