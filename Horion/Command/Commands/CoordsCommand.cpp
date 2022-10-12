#include "CoordsCommand.h"

CoordsCommand::CoordsCommand() : IMCCommand("coords", "Prints your coordinates", "") {
	registerAlias("pos");
}

CoordsCommand::~CoordsCommand() {
}

bool CoordsCommand::execute(std::vector<std::string>* args) {
	vec3_t* pos = g_Data.getLocalPlayer()->getPos();
	float yPos = pos->y - 1.62f;
	clientMessageF("[%sHorion%s] %sX: %s%.2f %sY: %s%.2f %sZ: %s%.2f", GOLD, WHITE, GREEN, GRAY, pos->x, GREEN, GRAY, yPos /* eye height */, GREEN, GRAY, pos->z);
	return true;
}
