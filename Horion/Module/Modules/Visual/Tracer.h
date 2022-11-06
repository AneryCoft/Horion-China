#pragma once

#include "../Module.h"
#include "../../../DrawUtils.h"

class Tracer : public IModule {
private:
	int red = 255;
	int green = 255;
	int blue = 255;
public:
	Tracer();
	~Tracer();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	//void onLevelRender() override;
	void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};