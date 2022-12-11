#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"

class Speed : public IModule {
private:
	float origSpeed = 0;
	float maxSpeed = 0.45f;
	float minSpeed = 0.38f;
	float speed;
	float vanillaSpeed = 1.0f;
	float lowhopMotion = 0.3f;
	float timer = 20.f;
	float duration = 1.f;

	bool inter = false;
	bool son = false;

	int index = 0;
	int stick = 0;

	SettingEnum mode;
	
public:
	Speed();
	~Speed();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
