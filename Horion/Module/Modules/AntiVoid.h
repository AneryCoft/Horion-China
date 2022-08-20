#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class AntiVoid : public IModule {
private:
	int distance = 5;
	bool onlyVoid = true;
	vec3_t savePos;
	vec3_t tempPos;

public:
	AntiVoid();
	~AntiVoid();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
