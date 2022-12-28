#pragma once
#include "../Module.h"
#include <combaseapi.h>

class RandomGameId : public IModule {
public:
	RandomGameId();
	~RandomGameId();

	static std::string GetUUID();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};