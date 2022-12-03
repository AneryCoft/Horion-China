#pragma once
#include "../Module.h"
#include "../../../Menu/Hud/ArrayList.h"

class ArrayListModule : public IModule {
public:
	bool clickToggle = true;
	bool keybinds = true;
	bool fill = true;
	bool border = true;

	ArrayListModule();
	~ArrayListModule();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};