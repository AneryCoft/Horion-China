#pragma once

#include "../Module.h"

class Crasher : public IModule {
private:
	SettingEnum mode;
public:
	Crasher();
	~Crasher();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
};
