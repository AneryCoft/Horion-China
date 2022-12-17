#include "NoHurtcam.h"

NoHurtcam::NoHurtcam() : IModule(0, Category::VISUAL, "Disables rotation of the camera when hit.") {
}

NoHurtcam::~NoHurtcam() {
}

const char* NoHurtcam::getModuleName() {
	return ("NoHurtcam");
}

uintptr_t NoHurtcam::getAddr() {
	return Utils::getBase() + 0x29B35E1;
}

void NoHurtcam::onEnable(){
	Utils::nopBytes((unsigned char*)getAddr(), 8);
}

void NoHurtcam::onDisable(){
	Utils::patchBytes((unsigned char*)getAddr(), (unsigned char*)"\x66\x0F\x6E\x8B\x3C\x07\x00\x00", 8);
}