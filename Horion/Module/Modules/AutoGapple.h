#pragma once

#include "Module.h"
#include "../../../SDK/CAttribute.h"

class AutoGapple : public IModule {
private:
	float health = 15.f;
	int itemID;
	bool rotation = false;
public:
	SettingEnum mode;

	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
