#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../Utils/TimerUtil.h"

class Breaker : public IModule {
private:
	SettingEnum action;
	bool beds = true;
	bool eggs = true;
	bool cakes = true;
	bool chests = false;
	bool barrels = false;
	bool redStone = false;
	bool targetEsp = false;
	float thick = 0.4f;
	bool rotations = false;
	float delay = 100.f;

	TimerUtil delayTime;
	bool shouldRotation = false;
	vec2_t angle = {};
	void findBlocks();
	uint8_t prevSlot = 0;
	void selectPickaxe();

public:
	std::vector<std::pair<vec3_ti, short>> blockList;
	std::vector<C_Entity*> entityBedList;

	int range = 5;
	bool treasures = true;
	bool lifeboatBeds = false;
	bool core = false;

	Breaker();
	~Breaker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onPlayerTick(C_Player* player) override;
};
