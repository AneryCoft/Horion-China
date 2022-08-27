#pragma once

#include "Module.h"

class BowSpam : public IModule {
private:
	int tick = 0;
	int delay = 0;
public:
	BowSpam();
	~BowSpam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
