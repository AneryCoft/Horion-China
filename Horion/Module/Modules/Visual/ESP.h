#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"

class ESP : public IModule {
public:
	SettingEnum target;
	SettingEnum renderMode;
	
	bool doRainbow = true;
	ESP();
	~ESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
