#pragma once

#include "../Module.h"

class Criticals : public IModule {
public:
	SettingEnum mode;

	Criticals();
	~Criticals();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onAttack(C_Entity*) override;
};
