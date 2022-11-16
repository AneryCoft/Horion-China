#pragma once
#include "../../ModuleManager.h"
#include "../../../DrawUtils.h"
#include "../Module.h"

class Swing : public IModule {
public:
    bool shouldBlock = false;
    SettingEnum swingMode;
    SettingEnum blockMode;
    bool autoBlock = false;

    Swing();
    ~Swing();

    virtual const char* getModuleName();
    virtual void onTick(C_GameMode* gm);
    virtual void onDisable();
};