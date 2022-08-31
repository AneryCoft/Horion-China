#pragma once

#include "Module.h"

class EditionFaker : public IModule {
public:
	int getFakedDevice();
	int getFakedInputMode();
	
	EditionFaker();
	~EditionFaker();

	SettingEnum device;
	SettingEnum inputMode;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* p, bool& cancelsend) override;
};
