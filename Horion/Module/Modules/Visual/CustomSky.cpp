#include "CustomSky.h"

CustomSky::CustomSky() : IModule(0, Category::VISUAL, "Change the color of the sky") {
	color = SettingEnum(this)
		.addEntry(EnumEntry("Rainbow", 0))
		.addEntry(EnumEntry("Dark", 1))
		.addEntry(EnumEntry("Custom", 2));
	registerEnumSetting("Color", &color, 0);
	registerIntSetting("Custom Red", &red, red, 0, 255);
	registerIntSetting("Custom Green", &green, green, 0, 255);
	registerIntSetting("Custom Blue", &blue, blue, 0, 255);
}

CustomSky::~CustomSky() {
}

const char* CustomSky::getModuleName() {
	return ("CustomSky");
}
