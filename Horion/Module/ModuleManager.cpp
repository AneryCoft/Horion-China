#include "ModuleManager.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = lockModuleListExclusive();
	moduleList.clear();
}

void ModuleManager::initModules() {
	logF("Initializing modules.");
	{
		auto lock = lockModuleListExclusive();

		moduleList.push_back(std::shared_ptr<IModule>(new HudModule()));
		moduleList.push_back(std::shared_ptr<IModule>(new Killaura()));
		moduleList.push_back(std::shared_ptr<IModule>(new ESP()));
		//moduleList.push_back(std::shared_ptr<IModule>(new Jetpack()));
		moduleList.push_back(std::shared_ptr<IModule>(new Aimbot()));
		moduleList.push_back(std::shared_ptr<IModule>(new TriggerBot()));
		moduleList.push_back(std::shared_ptr<IModule>(new StorageESP()));
		moduleList.push_back(std::shared_ptr<IModule>(new Velocity()));
		//moduleList.push_back(std::shared_ptr<IModule>(new RainbowSky()));
		moduleList.push_back(std::shared_ptr<IModule>(new AirJump()));
		moduleList.push_back(std::shared_ptr<IModule>(new Step()));
		moduleList.push_back(std::shared_ptr<IModule>(new Glide()));
		moduleList.push_back(std::shared_ptr<IModule>(new EditionFaker()));
		moduleList.push_back(std::shared_ptr<IModule>(new Phase()));
		moduleList.push_back(std::shared_ptr<IModule>(new Freecam()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoSprint()));
		moduleList.push_back(std::shared_ptr<IModule>(new Tracer()));
		moduleList.push_back(std::shared_ptr<IModule>(new BowAimbot()));
		moduleList.push_back(std::shared_ptr<IModule>(new Scaffold()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoFall()));
		moduleList.push_back(std::shared_ptr<IModule>(new Blink()));
		//moduleList.push_back(std::shared_ptr<IModule>(new CrystalPlace()));
		//moduleList.push_back(std::shared_ptr<IModule>(new CrystalBreak()));
		moduleList.push_back(std::shared_ptr<IModule>(new Nuker()));
		moduleList.push_back(std::shared_ptr<IModule>(new InstaBreak()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoPacket()));
		moduleList.push_back(std::shared_ptr<IModule>(new Speed()));
		moduleList.push_back(std::shared_ptr<IModule>(new Xray()));
		moduleList.push_back(std::shared_ptr<IModule>(new Breaker()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoSlowDown()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTotem()));
		moduleList.push_back(std::shared_ptr<IModule>(new InfiniteAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new ClickGuiMod()));
		moduleList.push_back(std::shared_ptr<IModule>(new ChestStealer()));
		moduleList.push_back(std::shared_ptr<IModule>(new Fly()));
		moduleList.push_back(std::shared_ptr<IModule>(new Hitbox()));
		moduleList.push_back(std::shared_ptr<IModule>(new Reach()));
		moduleList.push_back(std::shared_ptr<IModule>(new FullBright()));
		moduleList.push_back(std::shared_ptr<IModule>(new AirSwim()));
		moduleList.push_back(std::shared_ptr<IModule>(new InventoryMove()));
		moduleList.push_back(std::shared_ptr<IModule>(new HighJump()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoWeb()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastLadder()));
		moduleList.push_back(std::shared_ptr<IModule>(new StackableItem()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastEat()));
		moduleList.push_back(std::shared_ptr<IModule>(new Jesus()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoArmor()));
		moduleList.push_back(std::shared_ptr<IModule>(new BlockReach()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoClicker()));
		moduleList.push_back(std::shared_ptr<IModule>(new NameTags()));
		moduleList.push_back(std::shared_ptr<IModule>(new Criticals()));
		//moduleList.push_back(std::shared_ptr<IModule>(new Bhop()));
		moduleList.push_back(std::shared_ptr<IModule>(new Tower()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiVoid()));
		moduleList.push_back(std::shared_ptr<IModule>(new MidClick()));
		moduleList.push_back(std::shared_ptr<IModule>(new Teleport()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoFriends()));
		moduleList.push_back(std::shared_ptr<IModule>(new Spammer()));
		moduleList.push_back(std::shared_ptr<IModule>(new ChestAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiBot()));
		moduleList.push_back(std::shared_ptr<IModule>(new InventoryCleaner()));
		moduleList.push_back(std::shared_ptr<IModule>(new Derp()));
		moduleList.push_back(std::shared_ptr<IModule>(new Crasher()));
		moduleList.push_back(std::shared_ptr<IModule>(new CrystalAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new Timer()));
		//moduleList.push_back(std::shared_ptr<IModule>(new NightMode()));
		//moduleList.push_back(std::shared_ptr<IModule>(new NoSwing()));
		//moduleList.push_back(std::shared_ptr<IModule>(new CubeGlide()));
		//moduleList.push_back(std::shared_ptr<IModule>(new AirStuck()));
		moduleList.push_back(std::shared_ptr<IModule>(new Zoom()));
		moduleList.push_back(std::shared_ptr<IModule>(new Teams()));
		moduleList.push_back(std::shared_ptr<IModule>(new Nbt()));
		moduleList.push_back(std::shared_ptr<IModule>(new Godmode()));
		moduleList.push_back(std::shared_ptr<IModule>(new Freelook()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoSneak()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoHurtcam()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiImmobile()));
		moduleList.push_back(std::shared_ptr<IModule>(new Waypoints()));
		moduleList.push_back(std::shared_ptr<IModule>(new TimeChanger()));
		moduleList.push_back(std::shared_ptr<IModule>(new Spider()));
		moduleList.push_back(std::shared_ptr<IModule>(new Compass()));
		moduleList.push_back(std::shared_ptr<IModule>(new Radar()));
		moduleList.push_back(std::shared_ptr<IModule>(new VanillaPlus()));
		moduleList.push_back(std::shared_ptr<IModule>(new ViewModel()));
		//moduleList.push_back(std::shared_ptr<IModule>(new Twerk()));
		moduleList.push_back(std::shared_ptr<IModule>(new FollowPathModule()));
		moduleList.push_back(std::shared_ptr<IModule>(new FontChanger()));
		moduleList.push_back(std::shared_ptr<IModule>(new ClientTheme()));
		//moduleList.push_back(std::shared_ptr<IModule>(new Notifications()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoFish()));
		moduleList.push_back(std::shared_ptr<IModule>(new Disabler()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastStop()));
		moduleList.push_back(std::shared_ptr<IModule>(new InventoryView()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoGapple()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastDig()));
		moduleList.push_back(std::shared_ptr<IModule>(new BowSpam()));
		moduleList.push_back(std::shared_ptr<IModule>(new RandomLoginID()));
		moduleList.push_back(std::shared_ptr<IModule>(new Breadcrumbs()));
		moduleList.push_back(std::shared_ptr<IModule>(new Swing()));
		moduleList.push_back(std::shared_ptr<IModule>(new TPAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new TargetHud()));
		moduleList.push_back(std::shared_ptr<IModule>(new HackerDetector()));
		moduleList.push_back(std::shared_ptr<IModule>(new BlockOutline()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoLeave()));
		moduleList.push_back(std::shared_ptr<IModule>(new CustomSky()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoParticles()));
		moduleList.push_back(std::shared_ptr<IModule>(new Extinguisher()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTool()));
		moduleList.push_back(std::shared_ptr<IModule>(new ArrayListModule()));
		moduleList.push_back(std::shared_ptr<IModule>(new KillInsult()));
		//moduleList.push_back(std::shared_ptr<IModule>(new AntiAFK()));

#ifdef _DEBUG
		moduleList.push_back(std::shared_ptr<IModule>(new PacketLogger()));
		moduleList.push_back(std::shared_ptr<IModule>(new TestModule()));
#endif

		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{ *current->getModuleName() } < std::string{ *other->getModuleName() };
			});

		initialized = true;
	}

	getModule<ArrayListModule>()->setEnabled(getModule<HudModule>()->arraylist ? true : false);
	getModule<HudModule>()->setEnabled(true);
	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<AntiBot>()->setEnabled(true);
}

void ModuleManager::disable() {
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onLoadConfig(conf);
	}

	getModule<HudModule>()->setEnabled(true);
	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<AntiBot>()->setEnabled(true);
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onWorldTick(C_GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		if (mod->isEnabled() || mod->callWhenDisabled())
			try {
				[&] {__try {
				mod->onWorldTick(gameMode);
			} __except (EXCEPTION_EXECUTE_HANDLER) {} }();
			} catch (...) {
			}
	}
}

void ModuleManager::onTick(C_GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		if (mod->isEnabled() || mod->callWhenDisabled())
			try {
				[&] {__try {
				mod->onTick(gameMode);
			} __except (EXCEPTION_EXECUTE_HANDLER) {} }();
			} catch (...) {
			}
	}
}

void ModuleManager::onAttack(C_Entity* attackEnt) {
	if (!isInitialized())
		return;

	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		if (mod->isEnabled() || mod->callWhenDisabled())
			try {
			[&] {__try {
				mod->onAttack(attackEnt);
			} __except (EXCEPTION_EXECUTE_HANDLER) { } }();
			} catch (...) {
			}
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		try {
			[&] {__try {
			mod->onKeyUpdate(key, isDown);
		} __except (EXCEPTION_EXECUTE_HANDLER) { } }();
		} catch (...) {
		}
	}
}

void ModuleManager::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		if (mod->isEnabled() || mod->callWhenDisabled())
			try {
			[&] {__try {
				mod->onPreRender(renderCtx);
			} __except (EXCEPTION_EXECUTE_HANDLER) { } }();
			} catch (...) {
			}
	}
}

void ModuleManager::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		if (mod->isEnabled() || mod->callWhenDisabled())
			try {
			[&] {__try {
				mod->onPostRender(renderCtx);
			} __except (EXCEPTION_EXECUTE_HANDLER) { } }();
			} catch (...) {
			}
	}
}

void ModuleManager::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		auto look = std::shared_lock(it->iModuleLook);
		if (it->isEnabled() || it->callWhenDisabled())
			try {
			[&] {__try {
				it->onSendPacket(packet, cancelSend);
			} __except (EXCEPTION_EXECUTE_HANDLER) { } }();
			} catch (...) {
			}
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	return i;
}
void ModuleManager::onMove(C_MoveInputHandler* hand) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		auto look = std::shared_lock(it->iModuleLook);
		if (it->isEnabled() || it->callWhenDisabled())
			try {
			[&] {__try {
				it->onMove(hand);
			} __except (EXCEPTION_EXECUTE_HANDLER) { } }();
			} catch (...) {
			}
	}
}
void ModuleManager::onLevelRender() {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		auto look = std::shared_lock(it->iModuleLook);
		if (it->isEnabled() || it->callWhenDisabled())
			try {
			[&] {__try {
				it->onLevelRender();
			} __except (EXCEPTION_EXECUTE_HANDLER) {} }();
			} catch (...) {
			}
	}
}
void ModuleManager::onPlayerTick(C_Player* player) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		auto look = std::shared_lock(mod->iModuleLook);
		if (mod->isEnabled() || mod->callWhenDisabled())
			try {
			[&] {__try {
				mod->onPlayerTick(player);
			} __except (EXCEPTION_EXECUTE_HANDLER) {} }();
			} catch (...) {
			}
	}
}
void ModuleManager::onGetPickRange() {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		auto look = std::shared_lock(it->iModuleLook);
		if (it->isEnabled() || it->callWhenDisabled())
			try {
			[&] {__try {
				it->onGetPickRange();
			} __except (EXCEPTION_EXECUTE_HANDLER) {} }();
			} catch (...) {
			}
	}
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);
