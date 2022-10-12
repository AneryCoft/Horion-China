#pragma once
#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Module.h"
#include "../../../Memory/Hooks.h"

class Swing : public IModule {
private:
    bool reset = false;
public:
    bool shouldBlock = false;
    bool isAttacking = false;
    bool fakeBlock = false;
    SettingEnum mode;

    virtual const char* getModuleName();
    virtual void onTick(C_GameMode* gm);
    virtual void onDisable();
    Swing();
};