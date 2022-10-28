#include "BlockReach.h"

BlockReach::BlockReach() : IModule(0, Category::PLAYER, "Exteneded reach for placing/breaking blocks.") {
	registerFloatSetting(std::string("Reach"), &blockReach, blockReach, 5, 20);
}

BlockReach::~BlockReach() {
}

const char* BlockReach::getModuleName() {
	return ("BlockReach");
}
