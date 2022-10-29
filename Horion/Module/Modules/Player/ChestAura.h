#pragma once

#include "../Module.h"
#include "../../../../Utils/TimerUtil.h"

class ChestAura : public IModule {
private:
	int range = 3;
	bool enderchests = false;
	float delay = 100.f;
	TimerUtil delayTime;
public:
	//C_MoveInputHandler* inputHandler = nullptr;
	ChestAura();
	~ChestAura();

	std::vector<vec3_ti> chestlist;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
