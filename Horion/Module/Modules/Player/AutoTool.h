#pragma once
#include "../Module.h"

class AutoTool : public IModule {
public:
	AutoTool();
	~AutoTool();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};