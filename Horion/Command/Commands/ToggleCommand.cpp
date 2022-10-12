#include "ToggleCommand.h"

#include "../../Module/ModuleManager.h"

ToggleCommand::ToggleCommand() : IMCCommand("toggle", "Toggles a module", "<module>") {
	registerAlias("t");
}

ToggleCommand::~ToggleCommand() {
}

bool ToggleCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1);

	assertTrue(moduleName.size() > 0);

	auto modOpt = moduleMgr->getModuleByName(moduleName);
	if (!modOpt.has_value()) {
		clientMessageF("[%sHorion%s] %sCould not find Module with name: %s", GOLD, WHITE, RED, moduleName.c_str());
		return true;
	} 

	auto mod = modOpt.value();
	if (mod->isFlashMode()) 
		clientMessageF("[%sHorion%s] %sModule cannot be toggled!", GOLD, WHITE, RED);
	else {
		mod->toggle();
		clientMessageF("[%sHorion%s] %s%s is now %s", GOLD, WHITE, GREEN, mod->getModuleName(), mod->isEnabled() ? "enabled" : "disabled");
	}
		
	return true;
}
