#pragma once
#include "../Module.h"
#include "../../../DrawUtils.h"
#include "../../ModuleManager.h"

class InventoryView : public IModule {
public:
	int red = 255;
	int green = 255;
	int blue = 255;

	float xVal = 28.f;
	float invX = 0.f;
	float invY = 100.f;
	float opacity = 1.f;
	bool rainbow = false;
	float backGround = 0.5f;

	InventoryView();
	~InventoryView();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};