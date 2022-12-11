#pragma once

#include "../Module.h"

class EditionFaker : public IModule {
public:
	int getFakedDevice();
	int getFakedInputMode();
	
	SettingEnum device;
	SettingEnum inputMode;

	bool deviceModel = false;

	EditionFaker();
	~EditionFaker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
