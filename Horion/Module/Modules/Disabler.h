#pragma once
#include "Module.h"

class Disabler : public IModule {
public:
	Disabler();
	~Disabler();

	SettingEnum mode;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onTick(C_GameMode* gm) override;
};