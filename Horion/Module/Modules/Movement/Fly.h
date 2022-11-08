#pragma once

#include "../Module.h"

class Fly : public IModule {
private:
	float horizontalSpeed = 1.5f;
	float verticalSpeed = 0.5;
	bool groundSpoof = false;
	bool elytraSpoof = false;
	//bool damage = false;
	//int hiveSpeedIndex = 0;
	//int hiveVelocity = 0;
	//int enabledTick = 0;
	float timer = 20.f;

	int gameTick = 0;
	float y = 0.f;

public:
	Fly();
	~Fly();

	SettingEnum mode;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
};
