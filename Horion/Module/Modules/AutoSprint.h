#pragma once

#include "Module.h"

class AutoSprint : public IModule {
private:
	bool alldirections = true;

public:
	AutoSprint();
	~AutoSprint();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
