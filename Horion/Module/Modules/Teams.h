#pragma once

#include "Module.h"

class Teams : public IModule {
public:
	Teams();
	~Teams();

	SettingEnum method;
	bool antiRanks = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
