#pragma once

#include "../Module.h"
#include "../../../DrawUtils.h"

class BlockOutline : public IModule {
private:
	int red = 20;
	int green = 110;
	int blue = 200;
	float opacity = 1.f;
	float lineWidth = 0.5f;

public:
	BlockOutline();
	~BlockOutline();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};