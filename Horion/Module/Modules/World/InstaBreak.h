#pragma once

#include "../Module.h"

class InstaBreak : public IModule {
private:
	int tick = 0;
public:
	SettingEnum mode;

	InstaBreak();
	~InstaBreak();
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
