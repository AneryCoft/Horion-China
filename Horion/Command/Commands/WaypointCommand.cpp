#include "WaypointCommand.h"

WaypointCommand::WaypointCommand() : IMCCommand("waypoint", "Manage Waypoints", "<add|remove|teleport|removeall> <name> [x y z]") {
	registerAlias("wp");
}

WaypointCommand::~WaypointCommand() {
}

bool WaypointCommand::execute(std::vector<std::string>* args) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	assertTrue(player != nullptr);
	assertTrue(args->size() >= 2);

	static auto mod = moduleMgr->getModule<Waypoints>();

	std::string opt = args->at(1);

	if (opt == "removeall") {
		auto num = mod->getWaypoints()->size();
		mod->getWaypoints()->clear();
		clientMessageF("[%sHorion%s] %sRemoved %i waypoints!", GOLD, WHITE, GREEN, num);
		return true;
	}

	assertTrue(args->size() > 2);

	std::string name = args->at(2);
	name = Utils::sanitize(name);
	if (name.size() <= 1 || name.size() > 30) {
		clientMessageF("[%sHorion%s] %sInvalid name! Must be less than 30 characters!", GOLD, WHITE, RED);
		return true;
	}

	if (opt == "add") {
		vec3_t pos = player->currentPos.floor().add(0.5, 0, 0.5);
		if (args->size() == 6) {
			pos.x = assertFloat(args->at(3));
			pos.y = assertFloat(args->at(4));
			assertTrue(pos.y >= 0);
			pos.z = assertFloat(args->at(5));
		} else if (args->size() != 3) {
			return false;
		}
		int dimension;
		player->getDimensionId(&dimension);
		if (mod->add(name, pos, dimension)) {
			clientMessageF("[%sHorion%s] %sSuccessfully added waypoint \"%s\"", GOLD, WHITE, GREEN, name.c_str());
			if (!mod->isEnabled())
				clientMessageF("[%sHorion%s] %sEnable the waypoints module to see it ingame!", GOLD, WHITE, YELLOW);
		} else {
			clientMessageF("%sWaypoint \"%s\" already exists", RED, name.c_str());
		}
	} else if (opt == "remove") {
		if (mod->remove(name)) {
			clientMessageF("[%sHorion%s] %sRemoved waypoint \"%s\"", GOLD, WHITE, GREEN, name.c_str());
		} else {
			clientMessageF("[%sHorion%s] %sUnknown waypoint \"%s\"", GOLD, WHITE, RED, name.c_str());
		}
	}else if (opt == "tp" || opt == "teleport") {
		if (auto wp = mod->getWaypoint(name)) {
			auto wpV = wp.value();
			auto pos = wpV.pos;
			player->setPos(pos);
			clientMessageF("[%sHorion%s] %sTeleported to waypoint \"%s\" (%.02f, %.02f, %.02f)", GOLD, WHITE, GREEN, name.c_str(), pos.x, pos.y, pos.z);
		} else {
			clientMessageF("[%sHorion%s] %sUnknown waypoint \"%s\"", GOLD, WHITE, RED, name.c_str());
		}
	} else {
		return false;
	}

	return true;
}
