#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../../SDK/CAttribute.h"

class Breaker : public IModule {
private:
	int range = 5;
	bool beds = true;
	bool eggs = true;
	bool cakes = true;
	bool treasures = true;
	bool chests = false;
	bool barrels = false;
	bool redStone = false;
	bool lifeboatBeds = false;
	bool core = false;
	bool targetEsp = false;
	float thick = 0.4f;
	bool rotations = false;

	bool bedsRender = false;
	bool eggsRender = false;
	bool cakesRender = false;
	bool chestsRender = false;
	bool barrelsRender = false;
	bool redStoneRender = false;

	bool shouldRotation = false;
	vec3_ti renderPos;
	vec2_t angle;
	C_Entity* target;
	void selectPickaxe();
	bool shouldRenderEntity = false;

	int tick = 0;
	int delay = 0;

public:
	std::vector<vec3_ti> blockList;

	Breaker();
	~Breaker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onPlayerTick(C_Player* player) override;
};
