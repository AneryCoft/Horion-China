#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class Scaffold : public IModule {
private:
	SettingEnum mode;

	SettingEnum rotations;
	vec2_t angle = {};
	bool needRotation = false;

	SettingEnum autoBlocks;
	int prevSlot = 0;
	static uint8_t* renderItem;
	unsigned long oldProtect = 0;

	float length = 5.f;

	float timer = 20.f;
	
	bool showBlockCount = false;
	
	bool render = false;
	vec3_t blockPos = {};
	bool needRender = false;
	
	bool horizontal = false;
	float horizontalHigh = 0.f;

	bool tryScaffold(vec3_t blockBelow);
	
public:

	void selectedBlocks();
	int calcBlocksCount();

	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	//virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPlayerTick(C_Player* player) override;
	virtual void onSendPacket(C_Packet* packet, bool&) override;
	virtual void onGetPickRange() override;
};
