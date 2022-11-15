#pragma once

#include "../Module.h"
#include "../../../../SDK/CAttribute.h"
#include "../../../SDK/Tag.h"
#include "../../../Utils/TimerUtil.h"

class AutoGapple : public IModule {
private:
	bool targetItem(C_ItemStack* itemStack);
	void selectedItem();
	float health = 15.f;
	bool rotation = false;
	TimerUtil delayTime;
	float delay = 500.f;
	int tick = 0;
	bool canDelay = true;
	float maxHealth = 20.f;
	int prevSlot = 0;
public:
	SettingEnum item;

	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
