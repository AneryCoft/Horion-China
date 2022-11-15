#pragma once
#include <queue>
#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../Utils/TimerUtil.h"

struct packetAndTimer {
	NetworkLatencyPacket networkLatencyPacket;
	TimerUtil sendTime;
};

class Disabler : public IModule {
private:
	int tick = 0;
	TimerUtil attackTime;
public:
	SettingEnum mode;
	//std::vector<NetworkLatencyPacket*> NetworkLatencyPacketHolder = {};

	std::queue<packetAndTimer> packetQueue;

	Disabler();
	~Disabler();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onTick(C_GameMode* gm) override;
};
