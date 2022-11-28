#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/TimerUtil.h"
#include "../../../../SDK/CAttribute.h"
#include "../../../../Memory/Hooks.h"

class Killaura : public IModule
{
private:
	SettingEnum mode;
	SettingEnum priority;

	float FOV = 360.f;

	int CPS = 20;
	int maxCPS = 20;
	int minCPS = 20;
	TimerUtil attackTime;

	float switchDelay = 500.f;

	float rotationSpeed = 180.f;
	vec2_t localAngle = {};

	bool randomPitch = false;
	bool randomYaw = false;

	bool autoDisable = false;

	bool hurttime = false;

	bool autoweapon = false;
	void selectedWeapon();
	
	int switchTarget = 0;
	TimerUtil switchTime;
	C_Entity* lastTarget = nullptr;
	
	bool canswing = false;
	
	friend void findEntity(C_Entity* currentEntity, bool isRegularEntity);
	//static float randomFloat(float min, float max);
	
public:
	SettingEnum rotations;
	vec2_t angle = {};

	float range = 6.f;
	float swingRange = 6.5f;

	
	bool attackMob = false;
	bool throughBlock = true;

	bool targetListEmpty = false;
	bool isDigging = false;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPlayerTick(C_Player* player) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;	
};
