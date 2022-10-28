#pragma once
#include "../Module.h"

class FastStop : public IModule {
public:

	FastStop();
	~FastStop();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};