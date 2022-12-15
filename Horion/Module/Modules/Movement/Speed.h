#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"

class Speed : public IModule {
private:
	SettingEnum mode;

	float vanillaSpeed = 1.0f;
	float origSpeed = 0.f;

	float maxSpeed = 0.45f;
	float minSpeed = 0.38f;
	float speed = 0.f;
	
	float lowhopMotion = 0.3f;

	float duration = 1.f;
	float speedFriction = 0.65f;

	float timer = 20.f;
	
public:
	Speed();
	~Speed();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
