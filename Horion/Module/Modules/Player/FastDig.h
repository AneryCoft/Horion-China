#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class FastDig : public IModule {
public:
	float speed = 20.f;

	FastDig();
	~FastDig();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
