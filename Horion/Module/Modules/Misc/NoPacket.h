#pragma once

#include "../Module.h"

class NoPacket : public IModule {
public:
	NoPacket();
	~NoPacket();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	bool allowAutoStart() override;
	void onEnable() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
