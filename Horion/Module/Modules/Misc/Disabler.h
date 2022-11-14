#pragma once
#include <queue>
#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../../Utils/TimerUtil.h"
struct n_t {
	NetworkLatencyPacket nlp;
	TimerUtil tu;
};
class Disabler : public IModule {
private:
	int tick = 0;
	TimerUtil attackTime;
public:
	SettingEnum mode;
	//std::vector<NetworkLatencyPacket*> NetworkLatencyPacketHolder = {};

	std::queue<n_t> nt_queue;

	Disabler();
	~Disabler();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onTick(C_GameMode* gm) override;
};
