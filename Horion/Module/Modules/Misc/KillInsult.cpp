#include "KillInsult.h"

KillInsult::KillInsult() : IModule(0, Category::MISC, "Insult the player you kill") {
	registerBoolSetting("Mention", &mention, mention);
}

KillInsult::~KillInsult() {
}

const char* KillInsult::getModuleName() {
	return ("KillInsult");
}

void KillInsult::getInsultMessages() {
	static std::string path = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Horion-China\\InsultMessages.txt");

	if (!std::filesystem::exists(path)) {
		std::ofstream(path) << "Loser"; //创建文件
		InsultMessageVector.push_back("Loser");
	}
	else {
		std::ifstream file(path); //读取
		if (file) {
			std::string str;
			while (std::getline(file, str))
				InsultMessageVector.push_back(str);
		}
	}
}

void KillInsult::onAttack(C_Entity* attackedEnt) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	}

	if (Target::isValidTarget(attackedEnt)) {
		if (attackedEnt->isPlayer())
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
				std::string entityName = entity->playerName.getText();
				std::string insultText = InsultMessageVector[random(0, InsultMessageVector.size() - 1)];
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

void KillInsult::onEnable() {
	attackList.clear();
	getInsultMessages();
}