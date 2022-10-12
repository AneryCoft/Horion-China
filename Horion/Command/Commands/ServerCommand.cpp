#include "ServerCommand.h"

ServerCommand::ServerCommand() : IMCCommand("server", "Show information about the server you're playing on", "") {
	registerAlias("ip");
}

ServerCommand::~ServerCommand() {
}

bool ServerCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);

	if (g_Data.getRakNetInstance()->isonaServer())
		clientMessageF("[%sHorion%s] %sYou're currently playing on:\nIP: %s%s\n%sPort: %s%s", GOLD, WHITE, GREEN, GRAY, g_Data.getRakNetInstance()->serverIp.getText(), GREEN, GRAY, std::to_string(g_Data.getRakNetInstance()->serverPort).c_str());
	else
		clientMessageF("[%sHorion%s] %sYou're not playing on a server.", GOLD, WHITE, RED);
	return true;
}
