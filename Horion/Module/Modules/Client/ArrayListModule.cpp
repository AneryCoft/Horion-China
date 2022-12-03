#include "ArrayListModule.h"

ArrayListModule::ArrayListModule() : IModule(0, Category::CLIENT, "Set the ArrayList") {
	registerBoolSetting("ClickToggle", &clickToggle, clickToggle);
	registerBoolSetting("Show Keybinds", &keybinds, keybinds);
	registerBoolSetting("Fill", &fill, fill);
	registerBoolSetting("Border", &border, border);
}

ArrayListModule::~ArrayListModule() {
}

const char* ArrayListModule::getModuleName() {
	return ("ArrayList");
}