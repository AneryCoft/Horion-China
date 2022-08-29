#pragma once
#include "Module.h"
#include "../../DrawUtils.h"

class InventoryView : public IModule {
public:

	float rtext = 1.f;
	float gtext = 1.f;
	float btext = 1.f;

	float xVal = 28.f;
	float invX = 0.f;
	float invY = 100.f;
	float opacity = 1.f;
	float scale = 0.75f;
	bool RGB = false;

	InventoryView();
	~InventoryView();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};