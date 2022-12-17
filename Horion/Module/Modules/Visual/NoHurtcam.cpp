#include "NoHurtcam.h"

NoHurtcam::NoHurtcam() : IModule(0, Category::VISUAL, "Disables rotation of the camera when hit.") {
}

NoHurtcam::~NoHurtcam() {
}

const char* NoHurtcam::getModuleName() {
	return ("NoHurtcam");
}

uintptr_t NoHurtcam::getAddr() {
	return Utils::getBase() + 0X29B34E0;
}

void NoHurtcam::onEnable(){
//	if (!*buf) memcpy(buf, (void*)getAddr(), 5);
	Utils::patchBytes((unsigned char*)getAddr(), (unsigned char*)"\xc3\x90\x90\x90\x90", 5);
}

void NoHurtcam::onDisable(){
	Utils::patchBytes((unsigned char*)getAddr(), (unsigned char*)"\x48\x89\x5c\x24\x18", 5);
}