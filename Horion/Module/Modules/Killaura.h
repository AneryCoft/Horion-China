#pragma once

#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"
#include "../../../Utils/TimerUtil.h"
#include "../../../SDK/CAttribute.h"
#include "../../../Memory/Hooks.h"
#include <random>
#include <unordered_map>

class Killaura : public IModule
{
private:
	bool hurttime = false;
	bool autoweapon = false;
	void selectedWeapon();
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
	bool canswing = false;
	float swingRange = 6.5f;
	friend void findEntity(C_Entity* currentEntity, bool isRegularEntity);
	static float randomFloat(float min, float max);
	bool autoDisable = false;
	C_Entity* lastTarget = nullptr;
public:
	//bool rotations = false;
	bool targetListEmpty = false;
	vec2_t angle;
	bool isMobAura = false;
	bool throughBlock = true;
	bool isDigging = false;

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
