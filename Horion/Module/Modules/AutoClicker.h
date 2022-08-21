#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class AutoClicker : public IModule {
private:
	int maxcps = 20;
	int mincps = 20;
	//bool cpspertick = false;
	bool weapons = false;
	bool breakBlocks = false;
	SettingEnum LRclick = 0;
	bool playsound = true;
	bool fastplace = false;
	bool useitem = false;

private:
	float L_Odelay = 0.f;
	float R_Odelay = 0.f;
	int oldcps;
	int lcm;
	int clock_temp_R;
	int clock_temp_L;
	// float ftemp1;

public:
	AutoClicker();
	~AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onLevelRender() override;

	//virtual void onSendPacket(C_Packet* packet);
	virtual void onEnable() override;
	virtual void onDisable() override;

	// private:
	//	void LC_func();
	//	void RC_func();
};
