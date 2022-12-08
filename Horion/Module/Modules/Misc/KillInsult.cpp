#include "KillInsult.h"

KillInsult::KillInsult() : IModule(0, Category::MISC, "Insult the player you kill") {
	registerBoolSetting("Mention", &mention, mention);
}

KillInsult::~KillInsult() {
}

const char* KillInsult::getModuleName() {
	return ("KillInsult");
}

void KillInsult::onAttack(C_Entity* attackedEnt) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	}

	if (Target::isValidTarget(attackedEnt)) {
		attackList.insert(attackedEnt);
	}
}

void KillInsult::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr) {
		attackList.clear();
		return;
	}

	if (!attackList.empty()) {
		for (auto entity : attackList) {
			if (entity != nullptr && !entity->isAlive()) {
				std::string entityName = Utils::sanitize(Utils::onlyOneLine(entity->getNameTag()->getText()));
				std::string insultText = "Loser";
				std::string message = mention ? entityName + " " + insultText : insultText;

				C_TextPacket textPacket;
				textPacket.messageType = 1;
				textPacket.message.setText(message);
				textPacket.sourceName.setText(localPlayer->getNameTag()->getText());
				textPacket.xboxUserId = g_Data.getClientInstance()->minecraftGame->xuid;
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);

				eraseList.push_back(entity);
			}
		}

		if (!eraseList.empty()) {
			for (auto entity : eraseList) {
				attackList.erase(entity);
			}
			eraseList.clear();
		}
	}
}

void KillInsult::onDisable() {
	attackList.clear();
}