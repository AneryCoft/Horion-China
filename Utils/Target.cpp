#include "Target.h"

#include <regex>

#include "../Horion/Module/ModuleManager.h"
#include "../Memory/Hooks.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity* ent) {
	if (ent == nullptr)
		return false;

	auto localPlayer = g_Data.getLocalPlayer();

	if (ent == localPlayer)
		return false;

	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return false;

	if (!ent->checkNameTagFunc())
		return false;

	static auto antibot = moduleMgr->getModule<AntiBot>();
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	static auto teams = moduleMgr->getModule<Teams>();
	static auto noFriends = moduleMgr->getModule<NoFriends>();

	if (!ent->isAlive())
		return false;

	auto entityTypeId = ent->getEntityTypeId();

	if (antibot->isEntityIdCheckEnabled() && entityTypeId <= 130 && entityTypeId != 63)
		return false;

	if (entityTypeId == 63) {
		if (teams->isColorCheckEnabled()) {
			std::string targetName = ent->getNameTag()->getText();
			std::string localName = localPlayer->getNameTag()->getText();
			targetName = std::string(targetName, 0, targetName.find('\n'));
			localName = std::string(localName, 0, localName.find('\n'));

			auto colorTargetName = std::regex_replace(targetName, std::regex(u8"§r"), "");
			auto colorLocalName = std::regex_replace(localName, std::regex(u8"§r"), "");
			if (colorLocalName.find(u8"§") != std::string::npos &&
				colorTargetName.find(u8"§") != std::string::npos) {
				char colorTarget = colorTargetName.at(colorTargetName.rfind(u8"§") + 2);
				char colorLocal = colorLocalName.at(colorLocalName.rfind(u8"§") + 2);

				if (colorLocal == colorTarget)
					return false;
			}
		}
		if (teams->isAlliedCheckEnabled()) {
			if (localPlayer->isAlliedTo(ent)) return false;
		}
	}

	// Temporarily removed from gui, tons of false negatives
	if (antibot->isNameCheckEnabled() && !Target::containsOnlyASCII(ent->getNameTag()->getText()))
		return false;

	if (!noFriends->isEnabled() && FriendList::findPlayer(ent->getNameTag()->getText()))
		return false;

	if (antibot->isInvisibleCheckEnabled() && ent->isInvisible() )
		return false;

	if (antibot->isOtherCheckEnabled() && (ent->isSilent() || ent->isImmobile() || ent->getNameTag()->getTextLength() < 1 || std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos))
		return false;

	if (!hitboxMod->isEnabled() && antibot->isHitboxCheckEnabled())
		if ((ent->height < 1.5f || ent->width < 0.49f || ent->height > 2.1f || ent->width > 0.9f))
			return false;

	if (antibot->isExtraCheckEnabled() && !ent->canShowNameTag())
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
