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
	bool modeCheck = true;
	bool advanceCheck = true; //可以绕fap, ng, 反作弊检测或挂机假人 

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onWorldTick(C_GameMode*) override;
};
