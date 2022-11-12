#pragma once

#include "../Module.h"

class CustomSky : public IModule {
public:
	SettingEnum color;
	int red = 0;
	int green = 191;
	int blue = 255;

	CustomSky();
	~CustomSky();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
