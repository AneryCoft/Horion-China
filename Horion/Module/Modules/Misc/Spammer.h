#pragma once

#include "../Module.h"
#include "../../../../Utils/Utils.h"
#include "../../../../Memory/Hooks.h"
#include "../../../Utils/TimerUtil.h"

class Spammer : public IModule {
private:
	bool bypass = true;
	float delay = 500.f;
	TimerUtil delayTime;
	int length = 8;
	std::string message = "Horion China - A Minecraft bedrock Mod";

public:
	Spammer();
	~Spammer();

	inline std::string& getMessage() { return message; };
	inline float& getDelay() { return delay; };
	inline int& getLength() { return length; };
	inline bool& getBypass() { return bypass; };

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
