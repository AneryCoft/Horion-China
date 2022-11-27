#pragma once

#include "../Module.h"

class InstaBreak : public IModule {
public:
	SettingEnum mode;

	InstaBreak();
	~InstaBreak();
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
