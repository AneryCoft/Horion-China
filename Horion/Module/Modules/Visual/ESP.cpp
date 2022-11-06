#include "ESP.h"

#include "../../../../Memory/Hooks.h"

ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you.") {
	target = SettingEnum(this)
		.addEntry(EnumEntry("Player", 0))
		.addEntry(EnumEntry("Mob", 1))
		.addEntry(EnumEntry("Item", 2));
	registerEnumSetting("Target", &target, 0);
	renderMode = SettingEnum(this)
		.addEntry(EnumEntry("2D", 0))
		.addEntry(EnumEntry("3D Border", 1))
		.addEntry(EnumEntry("3D Box", 2));
	registerEnumSetting("RenderMode", &renderMode, 1);
	registerBoolSetting("Rainbow", &doRainbow, doRainbow);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

static float rcolors[4];

void doRenderStuff(C_Entity* ent, bool isRegularEntitie) {
	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	static auto espMod = moduleMgr->getModule<ESP>();

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	switch (espMod->target.selected) {
	case 0:
	{
		if (!Target::isValidTarget(ent))
			return;
	}
	break;
	case 1:
	{
		if (ent == localPlayer)
			return;

		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 319)
			return;

		if (!ent->isAlive())
			return;

		if (ent->timeSinceDeath > 0.f)
			return;

		if (!g_Data.getLocalPlayer()->canAttack(ent, false))
			return;
	}
	break;
	case 2:
	{
		if (ent->getEntityTypeId() != 64)
			return;
	}
	break;
	}

	if (espMod->doRainbow)
		DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
	else
		DrawUtils::setColor(0.9f, 0.9f, 0.9f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));


	float distance = (*localPlayer->getPos()).dist(*ent->getPos());
	switch (espMod->renderMode.selected) {
	case 0:
		DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, distance * 3.f)));
		break;
	case 1:
		DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, distance)));
		break;
	case 2:
		DrawUtils::drawBox(ent->aabb.lower, ent->aabb.upper, (float)fmax(0.2f, 1 / (float)fmax(1, distance)));
	}
}

void ESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() != nullptr && GameData::canUseMoveKeys()) {
		// Rainbow colors
		{
			if (rcolors[3] < 1) {
				rcolors[0] = 0.2f;
				rcolors[1] = 0.2f;
				rcolors[2] = 1.f;
				rcolors[3] = 1;
			}

			Utils::ApplyRainbow(rcolors, 0.0015f);
		}

		g_Data.forEachEntity(doRenderStuff);
	}
}
