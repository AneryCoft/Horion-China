#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class Hitbox : public IModule {
public:
	float height = 2.f;
	float width = 4.f;

	Hitbox();
	~Hitbox();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
