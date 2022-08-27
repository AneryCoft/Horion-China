#pragma once

#include "Module.h"

class Teams : public IModule {
public:
	Teams();
	~Teams();

	bool alliedCheck = false;
	bool colorCheck = true;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
