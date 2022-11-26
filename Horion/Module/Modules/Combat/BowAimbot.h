#pragma once

#include "../Module.h"
#include "../../../DrawUtils.h"
#include "../../ModuleManager.h"

class BowAimbot : public IModule {
private:
	bool silent = true;
	bool predict = false;
	bool visualize = false;
	vec2_t angle;
	float FOV = 180.f;
	SettingEnum priority;

public:
	bool throughBlock = false;

	BowAimbot();
	~BowAimbot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
