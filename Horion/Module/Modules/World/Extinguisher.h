#pragma once

#include "../Module.h"

class Extinguisher : public IModule {
private:
	int range = 3;
public:
	Extinguisher();
	~Extinguisher();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
