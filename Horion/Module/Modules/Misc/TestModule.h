#pragma once

#include "../Module.h"

class TestModule : public IModule {
private:
	int delay = 0;
	bool itemInfo = false;
	bool entityInfo = false;
	bool localInfo = false;
	bool blockInfo = false;
	bool killAura = false;
	
public:
	bool lagBackCheaker = false;
	float float1 = 0;
	int int1 = 0;
	bool bool1 = true;
	SettingEnum enum1;

	TestModule();
	~TestModule();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual bool isFlashMode() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) override;
	virtual void onMove(C_MoveInputHandler* hand) override;
	virtual void onDisable() override;
	void onLevelRender() override;
	virtual void onAttack(C_Entity* attackedEnt) override;
};
