#pragma once

#include "Module.h"

class Derp : public IModule {
private:
	int counter = 0;
	bool packetMode = false;
	SettingEnum mode;
public:
	Derp();
	~Derp();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPlayerTick(C_Player* player) override;
};
