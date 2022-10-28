#pragma once

#include "../Module.h"
#include "../../../../SDK/CAttribute.h"
#include "../../ModuleManager.h"

class AutoGapple : public IModule {
private:
	bool targetItem(int16_t itemId, uint16_t extraData);
	float health = 15.f;
	bool rotation = false;
	int delay = 10;
	int tick = 0;
	bool candelay = false;
	int duration;
	float maxHealth = 20.f;
public:
	SettingEnum item;

	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
