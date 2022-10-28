#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class CrystalAura : public IModule {
private:
	int prevSlot;
	int tick = 0;
	int delay = 0;
	bool autoPlace = true;
	bool preview = true;
	bool autoSelect = true;
	void selectCrystal();
	void placeCrystal(C_GameMode* gm, vec3_t* pos);
	vec3_ti bestPos;
	void destroyCrystal();
	SettingEnum mode;
public:
	float destroyRange = 3.5f;
	float placeRange = 5.f;
	int multiPlaceRange = 3;

	CrystalAura();
	~CrystalAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};