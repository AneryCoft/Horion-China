#include "SpammerCommand.h"

#include "../../Module/ModuleManager.h"

SpammerCommand::SpammerCommand() : IMCCommand("spammer", "Edit spammer delay/text", "<message/delay/bypass/length/manual> <string/float/bool>") {
	registerAlias("spam");
}

SpammerCommand::~SpammerCommand() {
}

bool SpammerCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);

	if (!g_Data.getLocalPlayer()->checkNameTagFunc())
		return false;

	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);

	auto spamMod = moduleMgr->getModule<Spammer>();

	if (option == "message") {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 1)
				os << " ";
			os << args->at(i);
		}
		std::string text = os.str().substr(1);
		spamMod->getMessage() = text;
		clientMessageF("[%sHorion%s] %sSpammer message set to %s%s%s!", GOLD, WHITE, GREEN, GRAY, text.c_str(), GREEN);
		return true;
	} else if (option == "delay") {
		float delay = assertInt(args->at(2));
		if (delay < 0.f) {
			clientMessageF("[%sHorion%s] %sDelay needs to be 1 or more!", GOLD, WHITE, RED);
			return true;
		} else {
			spamMod->getDelay() = delay;
			return true;
		}
	} else if (option == "bypass") {
		std::string data = args->at(2);
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		bool state = (data == "true") ? true : false;
		spamMod->getBypass() = state;
		clientMessageF("[%sHorion%s] %sBypass set to %s%s%s!", GOLD, WHITE, GREEN, GRAY, state ? "true" : "false", GREEN);
		return true;
	} else if (option == "length") {
		int length = assertInt(args->at(2));
		if (length < 1) {
			clientMessageF("[%sHorion%s] %sLength needs to be 1 or more!", GOLD, WHITE, RED);
			return true;
		} else {
			spamMod->getLength() = length;
			return true;
		}
	} else if (option == "manual") {
		int times = assertInt(args->at(2));
		std::ostringstream os;
		for (int i = 3; i < args->size(); i++) {
			if (i > 1)
				os << " ";
			os << args->at(i);
		}
		std::string text = os.str().substr(1);
		for (int i = 0; i < times; i++) {
			C_TextPacket textPacket;
			textPacket.messageType = 1;
			textPacket.message.setText(text + (spamMod->getBypass() ? (" | " + Utils::randomString(spamMod->getLength())) : ""));
			textPacket.sourceName = *g_Data.getLocalPlayer()->getNameTag();
			textPacket.xboxUserId = g_Data.getClientInstance()->minecraftGame->xuid;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		}
		return true;
	}
	return false;
}
