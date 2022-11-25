#pragma once

#include "../Module.h"

class Spider : public IModule {
private:
	float speed = 0.32f;
	//bool dontOvershoot = true;

public:
	Spider();
	~Spider();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
