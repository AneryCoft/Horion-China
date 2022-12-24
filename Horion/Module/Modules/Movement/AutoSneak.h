#pragma once

#include "../Module.h"
class AutoSneak : public IModule {
private:
	bool doSilent = false;
	bool canToggle = false;
	bool isSneak = false;

public:
	AutoSneak();
	~AutoSneak();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
