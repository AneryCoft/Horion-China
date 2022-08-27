#pragma once

#include "Module.h"

class AntiBot : public IModule {
private:

public:
	AntiBot();
	~AntiBot();

	bool hitboxCheck = true;
	bool nameCheck = true;
	bool invisibleCheck = true;
	bool entityIdCheck = true;
	bool moveCheck = true;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
