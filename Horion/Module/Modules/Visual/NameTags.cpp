#include "NameTags.h"

NameTags::NameTags() : IModule(0, Category::VISUAL, "Shows better nametags.") {
	registerBoolSetting("Underline", &underline, underline);
	registerBoolSetting("GameName", &gameName, gameName);
	registerBoolSetting("Armor", &displayArmor, displayArmor);
	registerBoolSetting("Distance", &displayDistance, displayDistance);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
}

NameTags::~NameTags() {
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

void drawNameTags(C_Entity* ent, bool) {
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (!ent->checkNameTagFunc())
		return;

	if (ent->getNameTag()->getTextLength() < 1)
		return;

	if (Target::isValidTarget(ent)) {
		//nameTagsMod->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
		//nameTagsMod->nameTags.insert(Utils::sanitize(Utils::onlyOneLine(ent->getNameTag()->getText())));
		float dist = ent->getPos()->dist(*g_Data.getLocalPlayer()->getPos());
		DrawUtils::drawNameTags(ent, fmax(0.8f, 3.f / dist));
		DrawUtils::flush();
	}
}

void NameTags::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	if (ingameNametagSetting != nullptr) {
		if (!gotPrevSetting) {
			lastSetting = *ingameNametagSetting;
			gotPrevSetting = true;
		}
		else {
			*ingameNametagSetting = gameName;  //修改原版游戏的玩家名字
		}
	}

	if (g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())
		return;

	g_Data.forEachValidEntity(drawNameTags);
}

void NameTags::onDisable() {
	if (!gameName) {
		if (*ingameNametagSetting != lastSetting) {
			*ingameNametagSetting = lastSetting;
			gotPrevSetting = false;
		}
	}
}
