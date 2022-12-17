#pragma once

#include "../Module.h"

class NoHurtcam : public IModule{
private:
	uintptr_t getAddr();

public:
	NoHurtcam();
	~NoHurtcam();
	
	const char* getModuleName() override;
	void onEnable() override;
	void onDisable() override;
 };
