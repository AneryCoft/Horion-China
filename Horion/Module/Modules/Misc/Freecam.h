#pragma once

#include "../Module.h"

class Freecam : public IModule {
private:
	vec3_t oldPos;
	vec3_t oldOffset = {};
	float speed = 0.325f;

	vec3_t pos;
	float pitch;
	float yaw;
	vec3_t velocity = vec3_t(0.f, 0.f, 0.f);
	//PlayerAuthInputPacket²ÎÊý±£´æ
public:
	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
