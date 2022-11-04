#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class AntiVoid : public IModule {
private:
	int distance = 5;
	bool onlyVoid = true;

public:
	SettingEnum mode;
	vec3_t savePos;
	bool isVoid();

	AntiVoid();
	~AntiVoid();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
