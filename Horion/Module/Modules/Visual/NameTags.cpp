#include "NameTags.h"

NameTags::NameTags() : IModule(0, Category::VISUAL, "Shows better nametags.") {
	registerBoolSetting("Underline", &underline, underline);
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
		nameTagsMod->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
		float dist = ent->getPos()->dist(*g_Data.getLocalPlayer()->getPos());
		DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist));
		DrawUtils::flush();
	}
}

void NameTags::onPreRender(C_MinecraftUIRenderContext* renderCtx) {

	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr || !GameData::canUseMoveKeys()) return;

	if (ingameNametagSetting)
		if (!gotPrevSetting) {
			lastSetting = *ingameNametagSetting;
			gotPrevSetting = true;
			*ingameNametagSetting = false;
		}
		else
			*ingameNametagSetting = false;  //disable other ppl's nametags

	g_Data.forEachValidEntity(drawNameTags);
}

void NameTags::onDisable() {
	if (ingameNametagSetting && gotPrevSetting) {
		*ingameNametagSetting = lastSetting;
		gotPrevSetting = false;
	}
}
