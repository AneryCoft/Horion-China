#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"
#include "../../../SDK/CAttribute.h"
#include <regex>

class TargetHud : public IModule {
private:
	bool displayPosition = true;
	bool displayDistance = true;
	bool displayHealth = true;
	bool displayArmorValue = true;

	bool attacked = false;
	C_Entity* target = nullptr;
	TimerUtil renderTime;
public:
	TargetHud();
	~TargetHud();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onAttack(C_Entity* attackedEnt) override;
};