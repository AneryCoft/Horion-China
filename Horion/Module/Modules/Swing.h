#pragma once
#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Module.h"
#include "../../../Memory/Hooks.h"

class Swing : public IModule {
private:
    bool reset = false;
public:
    float xPos = 0.f, yPos = 0.f, zPos = 0.f;
    bool shouldBlock = false;
    bool isAttacking = false;
    bool fakeBlock = false;
    SettingEnum mode;

    virtual const char* getModuleName();
    virtual void onTick(C_GameMode* gm);
    virtual void onDisable();
    Swing();
};