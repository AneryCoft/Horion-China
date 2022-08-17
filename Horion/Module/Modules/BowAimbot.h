#pragma once

#include "Module.h"

class BowAimbot : public IModule {
private:
	bool silent = true;
	bool predict = false;
	bool visualize = false;
	vec2_t angle;

public:
	BowAimbot();
	~BowAimbot();

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet) override;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
