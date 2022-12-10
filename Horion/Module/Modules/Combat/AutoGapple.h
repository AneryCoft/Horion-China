#pragma once

#include "../Module.h"
#include "../../../../SDK/CAttribute.h"
#include "../../../SDK/Tag.h"
#include "../../../Utils/TimerUtil.h"

class AutoGapple : public IModule {
private:
	SettingEnum item;
	int prevSlot = 0;
	void selectedItem();
	bool targetItem(C_ItemStack* itemStack);

	float health = 15.f;
	float maxHealth = 20.f;
	
	float delay = 500.f;
	TimerUtil delayTime;

	int tick = 0;
	bool rotation = false;
public:

	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onPlayerTick(C_Player* player) override;
};
