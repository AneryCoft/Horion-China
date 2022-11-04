#pragma once

#include "../Module.h"
#include "../../../../SDK/CAttribute.h"

class AutoLeave : public IModule {
private:
	float maxHealth = 20.f;
	float health = 5.f;

public:
	AutoLeave();
	~AutoLeave();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};