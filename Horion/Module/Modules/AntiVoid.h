#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class AntiVoid : public IModule {
private:
	int distance = 5;
	bool onlyVoid = true;
	vec3_t tempPos;

public:
	SettingEnum mode;
	bool lagBack = false;
	vec3_t savePos;

	AntiVoid();
	~AntiVoid();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
