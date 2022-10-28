#pragma once
#include "../Module.h"
#include <random>
#include <iomanip>

class RandomDeviceId : public IModule {
public:
	RandomDeviceId();
	~RandomDeviceId();

	static std::string GetUUID();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};