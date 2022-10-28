#pragma once
#include "../Module.h"

class AutoFish : public IModule {
private:
	int delay = 0;
	bool startDelay = false;
public:

	AutoFish();
	~AutoFish();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};