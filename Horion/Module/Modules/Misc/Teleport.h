#pragma once

#include "../Module.h"

class Teleport : public IModule {
private:
	bool hasClicked = false;
	bool onlyHand = false;
	bool shouldTP = false;
	bool onlySneak = true;
	vec3_t tpPos;
	vec3_t pos;
	SettingEnum mode;
public:
	Teleport();
	~Teleport();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
