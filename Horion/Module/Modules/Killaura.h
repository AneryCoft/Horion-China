#pragma once

#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"
#include "../../../Utils/TimerUtil.h"
#include "../../../SDK/CAttribute.h"

class Killaura : public IModule
{
private:
	bool hurttime = true;
	bool autoweapon = false;
	void findWeapon();
	int maxCPS = 20;
	int minCPS = 20;
	int CPS;
	float switchDelay = 500.f;
	int switchTarget = 0;
	float yawOffset = 0.f;
	float pitchOffset = 0.f;
	TimerUtil attackTime;
	TimerUtil switchTime;
	
	float FOV = 360.f;
	friend void findEntity(C_Entity* currentEntity, bool isRegularEntity);
public:
	//bool rotations = false;
	bool targetListEmpty = false;
	vec2_t angle;
	bool isMobAura = false;
	
	float range = 6.f;

	SettingEnum mode;
	SettingEnum rotations;
	SettingEnum priority;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onGetPickRange() override;
	virtual void onPlayerTick(C_Player* player) override;
};
