#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"

class Tower : public IModule {
private:
	float motion = 0.5f;
	bool tryTower(vec3_t blockBelow);
	bool rotations = false;
	int timer = 50;
	bool needRotations = false;
	bool autoBlock = false;
public:
	SettingEnum mode;
	Tower();
	~Tower();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPlayerTick(C_Player* player) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onDisable() override;
};
