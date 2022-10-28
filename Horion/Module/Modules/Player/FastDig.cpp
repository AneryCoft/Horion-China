#include "FastDig.h"

FastDig::FastDig() : IModule(0, Category::PLAYER, "Digging blocks faster") {
	registerFloatSetting("Speed", &speed, speed, 1.f, 20.f);
}

FastDig::~FastDig() {
}

const char* FastDig::getModuleName() {
	return ("FastDig");
}