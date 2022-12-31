#pragma once
#include "../Module.h"
#include <combaseapi.h>

class RandomLoginID : public IModule {
public:
	RandomLoginID();
	~RandomLoginID();

	static std::string GetUUID();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};