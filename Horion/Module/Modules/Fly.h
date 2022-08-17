#pragma once

#include "Module.h"

class Fly : public IModule {
private:
	float horizontalSpeed = 1.5f;
	float verticalSpeed = 0.5;

	int gameTick = 0;

public:
	Fly();
	~Fly();

	SettingEnum mode;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
