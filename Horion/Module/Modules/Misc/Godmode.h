#pragma once

#include "../Module.h"

class Godmode : public IModule {
private:

public:
	Godmode();
	~Godmode();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
