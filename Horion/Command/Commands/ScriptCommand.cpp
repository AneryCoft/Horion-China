#include "ScriptCommand.h"

ScriptCommand::ScriptCommand() : IMCCommand("script", "Manage scripts", "<load|unloadall>") {
}

ScriptCommand::~ScriptCommand() {
}

bool ScriptCommand::execute(std::vector<std::string>* args) {

	return false;
}
