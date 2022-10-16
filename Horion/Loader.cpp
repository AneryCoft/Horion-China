#include "Loader.h"

SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
bool isRunning = true;

#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif

DWORD WINAPI keyThread(LPVOID lpParam) {
	logF("Key thread started");

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if (keyMap == nullptr)
		throw std::exception("Keymap not allocated");

	auto clickMap = reinterpret_cast<uintptr_t>(malloc(5));
	if (clickMap == 0)
		throw std::exception("Clickmap not allocated");

	bool* keyMapAddr = nullptr;
	uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 89 1C B9");
	if (sigOffset != 0x0) {
		int offset = *reinterpret_cast<int*>((sigOffset + 3));                                   // Get Offset from code
		keyMapAddr = reinterpret_cast<bool*>(sigOffset + offset + /*length of instruction*/ 7);  // Offset is relative
	} else {
		logF("!!!KeyMap not located!!!");
		throw std::exception("Keymap not located");
	}

	C_HIDController** hidController = g_Data.getHIDController();

	while (isRunning) {
		if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::shouldTerminate()) {  // Press L to uninject
			isRunning = false;
			break;
		}

		for (uintptr_t i = 0; i < 0xFF; i++) {
			bool* newKey = keyMapAddr + (4 * i);
			bool newKeyPressed = (*newKey) && GameData::canUseMoveKeys();  // Disable Keybinds when in chat or inventory
			bool* oldKey = keyMap + (4 * i);
			if (newKeyPressed != *oldKey) {
				moduleMgr->onKeyUpdate((int)i, newKeyPressed);
			}
			if (*newKey != *oldKey) {  // Skip Chat or inventory checks
				TabGui::onKeyUpdate((int)i, *newKey);
				ClickGui::onKeyUpdate((int)i, *newKey);
			}
		}

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);

		Sleep(2);
	}
	logF("Stopping Threads...");
	GameData::terminate();
	Sleep(200);  // Give the threads a bit of time to exit

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1);  // Uninject
}


#ifndef _MSC_VER
#define _MSC_VER "unk"
#endif

DWORD WINAPI start(LPVOID lpParam) {
	logF("Starting up...");
	logF("MSC v%i at %s", _MSC_VER, __TIMESTAMP__);

	init();

	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full)) {
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe");  // Get Module for Base Address

	MH_Initialize();
	GameData::initGameData(gameModule, &mem, (HMODULE)lpParam);
	Target::init(g_Data.getPtrLocalPlayer());

	Hooks::Init();

	DWORD keyThreadId;
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, &keyThreadId);  // Checking Keypresses
	logF("KeyT: %i", keyThreadId);

	/*logF("Waiting for injector");
	while (!g_Data.isInjectorConnectionActive()) {
		Sleep(10);
		if (!isRunning)
			ExitThread(0);
	}
	logF("Injector found");*/

	cmdMgr->initCommands();
	logF("Initialized command manager (1/3)");
	moduleMgr->initModules();
	logF("Initialized module manager (2/3)");
	configMgr->init();
	logF("Initialized config manager (3/3)");

	Hooks::Enable();
	TabGui::init();
	ClickGui::init();

	logF("Hooks enabled");

	std::thread countThread([] {
		while (isRunning) {
			Sleep(1000);
			g_Data.fps = g_Data.frameCount / 3;
			g_Data.frameCount = 0;
			g_Data.cpsLeft = g_Data.leftclickCount;
			g_Data.leftclickCount = 0;
			g_Data.cpsRight = g_Data.rightclickCount;
			g_Data.rightclickCount = 0;
		}
	});
	countThread.detach();

	/*
	std::thread newThread([] {
		Sleep(500); //wait for ModuleManager()
		while (isRunning) {
			Sleep(10);
			moduleMgr->onUpdate();
		}
	});
	newThread.detach();
	*/

	logF("Count thread started");

	ExitThread(0);
}

BOOL __stdcall DllMain(HMODULE hModule,
					   DWORD ul_reason_for_call,
					   LPVOID) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	} break;
	case DLL_PROCESS_DETACH:
		isRunning = false;
		Utils::makeRoamingFoldr("Horion-China");
		scriptMgr.unloadAllScripts();
		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();
		//GameWnd.OnDeactivated();

		logF("Removing logger");
		Logger::Disable();

		MH_Uninitialize();
		delete moduleMgr;
		delete cmdMgr;
		delete configMgr;
		if (g_Data.getLocalPlayer() != nullptr) {
			C_GuiData* guiData = g_Data.getClientInstance()->getGuiData();
			if (guiData != nullptr && !GameData::shouldHide())
				guiData->displayClientMessageF("%sEjected!", RED);
		}
		break;
	}
	return TRUE;
}
