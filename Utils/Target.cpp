#include "Target.h"

#include <regex>

#include "../Horion/Module/ModuleManager.h"
#include "../Memory/Hooks.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity* ent) {
	static auto antibotMod = moduleMgr->getModule<AntiBot>();
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	static auto teamsMod = moduleMgr->getModule<Teams>();
	static auto noFriendsMod = moduleMgr->getModule<NoFriends>();

	auto localPlayer = g_Data.getLocalPlayer();

	if (ent == nullptr)
		return false;

	if (ent == localPlayer)
		return false;

	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return false;

	if (!ent->checkNameTagFunc())
		return false;

	if (!ent->isAlive())
		return false;

	auto entityTypeId = ent->getEntityTypeId();

	if (antibotMod->isEnabled()) {
		if (antibotMod->hitboxCheck) {
			/*if ((ent->height < 1.5f || ent->width < 0.49f || ent->height > 2.1f || ent->width > 0.9f))
				return false;*/
			if (hitboxMod->isEnabled()) {
				if (ent->height > hitboxMod->height || ent->width > hitboxMod->width)
					return false;

			}
			else {
				if (ent->height > 1.8f || ent->width > 0.6f)
					return false;
			}
		}

		if (antibotMod->nameCheck) {
			if (ent->getNameTag()->getTextLength() < 1)
				return false;

			if (!ent->canShowNameTag())
				return false;

			if (!Target::containsOnlyASCII(ent->getNameTag()->getText()))
				return false; // Temporarily removed from gui, tons of false negatives

			if (std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos)
				return false;

			if (!ent->checkNameTagFunc())
				return false;
		}

		if (antibotMod->invisibleCheck) {
			if (ent->isInvisible())
				return false;
		}

		if (antibotMod->entityIdCheck) {
			if (entityTypeId != 319)
				return false;
		}
		if (antibotMod->moveCheck) {
			if (ent->isSilent())
				return false;

			if (ent->isImmobile())
				return false;
		}
		if (antibotMod->modeCheck) {
			if (ent->gamemode == 1)
				return false; //不攻击创造模式的玩家
		}
	}

	if (teamsMod->isEnabled()) {
		if (entityTypeId == 319) {
			switch (teamsMod->method.selected) {
			case 0: {
				if (localPlayer->isAlliedTo(ent))
					return false;
			}break;
			case 1: {
				std::string targetName = ent->getNameTag()->getText();
				std::string localName = localPlayer->getNameTag()->getText();

				if (targetName.size() > 2 && localName.size() > 2){
					targetName = std::string(targetName, 0, targetName.find('\n'));
					localName = std::string(localName, 0, localName.find('\n'));

					//logF("%s", targetName.data());

					auto colorTargetName = std::regex_replace(targetName, std::regex(u8"§r"), "");
					auto colorLocalName = std::regex_replace(localName, std::regex(u8"§r"), "");

					if (teamsMod->antiRanks) {
						char colorTarget = colorTargetName[colorTargetName.rfind(u8"§") + 2];
						char colorLocal = colorLocalName[colorLocalName.rfind(u8"§") + 2];
						//char colorTarget = colorTargetName.at(colorTargetName.rfind(u8"§") + 2);
						//char colorLocal = colorLocalName.at(colorLocalName.rfind(u8"§") + 2);

						if (colorLocal == colorTarget)
							return false;
					}
					else {
						char colorTarget = colorTargetName[colorTargetName.find(u8"§") + 2];
						char colorLocal = colorLocalName[colorLocalName.find(u8"§") + 2];
						//char colorTarget = colorTargetName.at(colorTargetName.find(u8"§") + 2);
						//char colorLocal = colorLocalName.at(colorLocalName.find(u8"§") + 2);

						//logF("%c", colorTarget);

						if (colorLocal == colorTarget)
							return false;
					}
				}
			}break;
			case 2: {
				if (localPlayer->getArmorColorInSlot(0, 0) == ent->getArmorColorInSlot(0, 0))
					return false;
			}
			}
		}
	}

	if (!noFriendsMod->isEnabled() && FriendList::findPlayer(ent->getNameTag()->getText()))
		return false;

	return true;
}

bool Target::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}
