#pragma once
#include "../Module.h"
#include <combaseapi.h>

class RandomConnectionId : public IModule {
public:
	RandomConnectionId();
	~RandomConnectionId();

	static std::string GetUUID();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};