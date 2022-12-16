#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../DrawUtils.h"

class Tower : public IModule {
private:
	float motion = 0.32f;
	bool rotations = false;
	float timer = 50.f;
	bool needRotations = false;
	bool autoBlocks = false;
	bool renderBlocks = false;
	SettingEnum mode;
public:
	
	Tower();
	~Tower();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPlayerTick(C_Player* player) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
};
