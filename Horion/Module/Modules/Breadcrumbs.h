#pragma once
#include "Module.h"
#include "../../DrawUtils.h"

class Breadcrumbs : public IModule {
private:
	std::vector<vec3_t> points;

public:
	Breadcrumbs();
	~Breadcrumbs();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
};