#pragma once
#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Module.h"

class FluxSwing : public IModule {
private:
    bool reset = false;
public:
    float xPos = 0.f, yPos = 0.f, zPos = 0.f;
    bool shouldBlock = false;
    bool isAttacking = false;
    bool fkblock = false;

    virtual const char* getModuleName();
    virtual void onTick(C_GameMode* gm);
    virtual void onDisable();
    virtual void onEnable();
    FluxSwing();
};