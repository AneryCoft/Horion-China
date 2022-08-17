#pragma once

#include "Module.h"

class Tracer : public IModule {
public:
	Tracer();
	~Tracer();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	void onLevelRender() override;
	void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};