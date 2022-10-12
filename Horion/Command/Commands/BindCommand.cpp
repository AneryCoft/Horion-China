#include "BindCommand.h"

#include "../../Module/ModuleManager.h"

BindCommand::BindCommand() : IMCCommand("bind", "Binds modules to specific keys", "<module> <key>") {
	registerAlias("b");
}

BindCommand::~BindCommand() {
}

bool BindCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1);
	assertTrue(moduleName.size() > 0);

	auto modOpt = moduleMgr->getModuleByName(moduleName);
	if (!modOpt.has_value()) {
		clientMessageF("[%sHorion%s] %sCould not find module with name: %s%s", GOLD, WHITE, RED, GRAY, moduleName.c_str());
		return true;
	}
	auto mod = modOpt.value();

	if (args->size() >= 3) {
		std::string key = args->at(2);

		assertTrue(key.size() > 0);

		if (key.size() > 1) {
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			const char* needle = key.c_str();

			if (key == "none") {
				mod->setKeybind(0x0);
				clientMessageF("[%sHorion%s] %sSuccessfully unbound %s%s%s!", GOLD, WHITE, GREEN, GRAY, mod->getModuleName(), GREEN);
				return true;
			}

			for (int i = 0; i < 190; i++) {
				const char* haystack = KeyNames[i];
				size_t len = strlen(needle) + 1;
				char* haystackLowercase = new char[len];
				for (int i = 0; i < len; i++)
					haystackLowercase[i] = tolower(haystack[i]);

				if (strcmp(needle, haystackLowercase) == 0) {
					mod->setKeybind(i);
					clientMessageF("[%sHorion%s] %sThe keybind of %s%s %sis now %s'%s'", GOLD, WHITE, GREEN, GRAY, mod->getModuleName(), GREEN, GRAY, haystack);
					delete[] haystackLowercase;
					return true;
				}
				delete[] haystackLowercase;
			}
			clientMessageF("[%sHorion%s] %sInvalid key!", GOLD, WHITE, RED);
			return true;
		}

		int keyCode = (int)key.at(0);
		if (keyCode >= 0x61 && keyCode <= 0x7A)  // Convert key to lower case
			keyCode -= 0x20;

		if (keyCode >= 0x30 && keyCode <= 0x5A) {
			auto modOpt = moduleMgr->getModuleByName(moduleName);
			if (!modOpt.has_value()) {
				clientMessageF("[%sHorion%s] %sCould not find module with name: %s%s", GOLD, WHITE, RED, GRAY, moduleName.c_str());
			}
			else {
				modOpt.value()->setKeybind(keyCode);
				clientMessageF("[%sHorion%s] %sThe Keybind of %s%s %sis now %s'%c'", GOLD, WHITE, GREEN, GRAY, mod->getModuleName(), GREEN, GRAY, keyCode);
			}
		}
		else {
			clientMessageF("[%sHorion%s] %sInvalid Key! Outside of ASCII range: %s%X", GOLD, WHITE, RED, GRAY, keyCode);
		}
		return true;
	}
	else {
		clientMessageF("[%sHorion%s] %s%s is currently bound to %s%s", GOLD, WHITE, GRAY, mod->getModuleName(), RED, GRAY, Utils::getKeybindName(mod->getKeybind()));
		return true;
	}

}
