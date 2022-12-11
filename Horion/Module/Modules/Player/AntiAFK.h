#pragma once

#include "../Module.h"

class AntiAFK : public IModule {
private:

public:
	AntiAFK();
	~AntiAFK();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};