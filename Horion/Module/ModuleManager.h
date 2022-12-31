#pragma once

#include <typeinfo>
#include <vector>
#include <optional>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "../../Memory/GameData.h"

#include "Modules/Combat/Aimbot.h"
#include "Modules/Combat/AutoClicker.h"
#include "Modules/Combat/AutoGapple.h"
#include "Modules/Combat/BowAimbot.h"
#include "Modules/Combat/BowSpam.h"
#include "Modules/Combat/Criticals.h"
#include "Modules/Combat/CrystalAura.h"
#include "Modules/Combat/Hitbox.h"
#include "Modules/Combat/InfiniteAura.h"
#include "Modules/Combat/Killaura.h"
#include "Modules/Combat/Reach.h"
#include "Modules/Combat/TPAura.h"
#include "Modules/Combat/TriggerBot.h"

#include "Modules/Visual/BlockOutline.h"
#include "Modules/Visual/Breadcrumbs.h"
#include "Modules/Visual/CustomSky.h"
#include "Modules/Visual/ESP.h"
#include "Modules/Visual/Freelook.h"
#include "Modules/Visual/FullBright.h"
#include "Modules/Visual/NameTags.h"
#include "Modules/Visual/NoHurtcam.h"
#include "Modules/Visual/NoParticles.h"
#include "Modules/Visual/StorageESP.h"
#include "Modules/Visual/Swing.h"
#include "Modules/Visual/TargetHud.h"
#include "Modules/Visual/TimeChanger.h"
#include "Modules/Visual/Tracer.h"
#include "Modules/Visual/ViewModel.h"
#include "Modules/Visual/Waypoints.h"
#include "Modules/Visual/Xray.h"
#include "Modules/Visual/Zoom.h"

#include "Modules/Movement/AirJump.h"
#include "Modules/Movement/AirSwim.h"
#include "Modules/Movement/AntiVoid.h"
#include "Modules/Movement/AutoSneak.h"
#include "Modules/Movement/AutoSprint.h"
#include "Modules/Movement/FastLadder.h"
#include "Modules/Movement/FastStop.h"
#include "Modules/Movement/Fly.h"
#include "Modules/Movement/FollowPathModule.h"
#include "Modules/Movement/Glide.h"
#include "Modules/Movement/HighJump.h"
#include "Modules/Movement/InventoryMove.h"
#include "Modules/Movement/Jesus.h"
#include "Modules/Movement/NoSlowDown.h"
#include "Modules/Movement/NoWeb.h"
#include "Modules/Movement/Phase.h"
#include "Modules/Movement/Speed.h"
#include "Modules/Movement/Spider.h"
#include "Modules/Movement/Step.h"
#include "Modules/Movement/Velocity.h"

//#include "Modules/Player/AntiAFK.h"
#include "Modules/Player/AntiBot.h"
#include "Modules/Player/AutoArmor.h"
#include "Modules/Player/AutoFish.h"
#include "Modules/Player/AutoLeave.h"
#include "Modules/Player/AutoTool.h"
#include "Modules/Player/AutoTotem.h"
#include "Modules/Player/BlockReach.h"
#include "Modules/Player/ChestStealer.h"
#include "Modules/Player/ChestAura.h"
#include "Modules/Player/FastDig.h"
#include "Modules/Player/FastEat.h"
#include "Modules/Player/InventoryCleaner.h"
#include "Modules/Player/MidClick.h"
#include "Modules/Player/Nbt.h"
#include "Modules/Player/NoFall.h"
#include "Modules/Player/NoFriends.h"
#include "Modules/Player/StackableItem.h"
#include "Modules/Player/Teams.h"

#include "Modules/World/Breaker.h"
#include "Modules/World/Extinguisher.h"
#include "Modules/World/InstaBreak.h"
#include "Modules/World/Nuker.h"
#include "Modules/World/Scaffold.h"
#include "Modules/World/Tower.h"

#include "Modules/Misc/AntiImmobile.h"
#include "Modules/Misc/Blink.h"
#include "Modules/Misc/Crasher.h"
#include "Modules/Misc/Derp.h"
#include "Modules/Misc/Disabler.h"
#include "Modules/Misc/EditionFaker.h"
#include "Modules/Misc/Freecam.h"
#include "Modules/Misc/Godmode.h"
#include "Modules/Misc/HackerDetector.h"
#include "Modules/Misc/KillInsult.h"
#include "Modules/Misc/NoPacket.h"
#include "Modules/Misc/RandomLoginID.h"
#include "Modules/Misc/Spammer.h"
#include "Modules/Misc/Teleport.h"
#include "Modules/Misc/Timer.h"
#include "Modules/Misc/VanillaPlus.h"
#ifdef _DEBUG
#include "Modules/Misc/PacketLogger.h"
#include "Modules/Misc/TestModule.h"
#endif

#include "Modules/Client/ArrayListModule.h"
#include "Modules/Client/ClickGuiMod.h"
#include "Modules/Client/ClientTheme.h"
#include "Modules/Client/Compass.h"
#include "Modules/Client/FontChanger.h"
#include "Modules/Client/HudModule.h"
#include "Modules/Client/InventoryView.h"
#include "Modules/Client/Radar.h"

#include "Modules/Module.h"

class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(C_GameMode* gameMode);
	void onAttack(C_Entity* attackedEnt);
	void onWorldTick(C_GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onMove(C_MoveInputHandler* handler);
	void onSendPacket(C_Packet* packet, bool& cancelSend);
	void onPlayerTick(C_Player* player);
	void onGetPickRange();
	
	//void onUpdate();
	
	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(moduleListMutex); }

	std::shared_mutex* getModuleListLock() { return &moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows:
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>();
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !
	 */
	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		auto lock = lockModuleList();
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())) {

				return pRet;
			}
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

		auto lock = lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager* moduleMgr;
