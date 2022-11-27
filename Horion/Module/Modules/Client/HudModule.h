#pragma once

#include "../Module.h"
#include "../../../DrawUtils.h"
#include "../../../Scripting/ScriptManager.h"
#include "../../../Menu/Hud/TabGui.h"
#include "../../../Menu/Hud/ArrayList.h"
#include "../../../Menu/Hud/Notifications.h"
#include "../../../Menu/Hud/Watermark.h"

class HudModule : public IModule {
public:
	HudModule();
	~HudModule();

	bool tabgui = true;
	bool arraylist = true;
	bool clickToggle = true;
	bool watermark = true;
	bool coordinates = false;
	bool keybinds = true;
	bool displayArmor = true;
	bool keystrokes = true;
	bool fps = true;
	bool cps = true;
	bool alwaysShow = false;
	bool angle = false;
	bool speed = true;
	bool notifications = true;

	float scale = 1.f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	static void drawKeystroke(char key, const vec2_t& pos);
	static void drawLeftMouseKeystroke(vec2_t pos);
	static void drawRightMouseKeystroke(vec2_t pos);
};
