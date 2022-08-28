#include "Teams.h"

Teams::Teams() : IModule(0, Category::COMBAT, "Don't attack team members!") {
	method = SettingEnum(this)
		.addEntry(EnumEntry("Allied", 0))
		.addEntry(EnumEntry("NameColor", 1))
		.addEntry(EnumEntry("ArmorColor", 2));
	registerEnumSetting("Mode", &method, 1);
	registerBoolSetting("AntiRanks", &antiRanks, antiRanks);
}

Teams::~Teams() {
}

const char* Teams::getModuleName() {
	return ("Teams");
}
