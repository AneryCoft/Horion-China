#pragma once

#include "Module.h"

class NoFall : public IModule {
private:
	float distanceToGround();
	bool selectWaterBucket();
	vec3_ti blockPos;
	int prevSlot;
public:
	SettingEnum mode;

	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onTick(C_GameMode* gm) override;
};