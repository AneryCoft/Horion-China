#include "ClientTheme.h"
ClientTheme::ClientTheme() : IModule(0, Category::CLIENT, "Go for a retro style or streamline.") {
	Theme = SettingEnum(this)
		.addEntry(EnumEntry("Light", 0))
		.addEntry(EnumEntry("Dark", 1))
		.addEntry(EnumEntry("Legacy", 2));
	registerEnumSetting("Client Theme", &Theme, 0);
}

ClientTheme::~ClientTheme() {
}

const char* ClientTheme::getModuleName() {
	return ("ClientTheme");
}