#pragma once

#include "../Module.h"
#include "../../../DrawUtils.h"

class BlockOutline : public IModule {
private:
	int red = 28;
	int green = 110;
	int blue = 200;
	float opacity = 1.f;
	float lineWidth = 0.5f;
	bool rainbow = false;
	bool outline = true;

public:
	BlockOutline();
	~BlockOutline();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};