#pragma once

#include "../../Module/ModuleManager.h"
#include "../../DrawUtils.h"
#include <vector>
#include <Windows.h>
#include "../../Scripting/ScriptManager.h"

class TabGui {
private:
	static void renderLabel(const char* text, std::shared_ptr<IModule> mod = 0);
	static void renderLevel();

public:
	static void init();
	static void renderTabGui();
	static void onKeyUpdate(int key, bool isDown);
};
