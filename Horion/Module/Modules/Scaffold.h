#pragma once

#include "Module.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"

class Scaffold : public IModule {
private:
	//bool spoof = false;
	bool tryScaffold(vec3_t blockBelow);
	int length = 5;
	bool showBlockCount = false;
	bool autoBlock = false;
	int prevSlot = 0;
	float horizontalHigh;
	bool rotations = false;
	bool render = false;
	vec3_t blockPos;
	vec3_t renderPos;
	bool needRender = false;
	bool needRotations = false;
public:
	SettingEnum mode;
	bool findBlock();
	int calcCount();
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onPlayerTick(C_Player* player) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onEnable() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onGetPickRange() override;
	//virtual void onTick(C_GameMode*) override;
};
