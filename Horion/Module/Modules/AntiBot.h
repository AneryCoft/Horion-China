#pragma once

#include "Module.h"
#include "../../../Utils/TimerUtil.h"
#include "../../../Memory/Hooks.h"
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
	bool advanceCheck = false; //可以绕fap, ng, 反作弊检测或挂机假人
	bool nameCheckPlus = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onWorldTick(C_GameMode*) override;
};
