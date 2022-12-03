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

	if (ent->damageTime > 10 || ent->damageTime < 0)
		ent->damageTime = 0;
		//部分实体的HurtTime是脏数据

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
		break;
	case 3:
	{
		/*
		vec2_t boxPos1 = DrawUtils::worldToScreen(ent->aabb.lower);
		vec2_t boxPos2 = DrawUtils::worldToScreen(vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
		vec2_t boxPos3 = DrawUtils::worldToScreen(vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
		vec2_t boxPos4 = DrawUtils::worldToScreen(vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
		vec2_t boxPos5 = DrawUtils::worldToScreen(ent->aabb.upper);
		vec2_t boxPos6 = DrawUtils::worldToScreen(vec3_t(ent->aabb.lower.x, ent->aabb.upper.y, ent->aabb.lower.z));
		vec2_t boxPos7 = DrawUtils::worldToScreen(vec3_t(ent->aabb.lower.x, ent->aabb.upper.y, ent->aabb.upper.z));
		vec2_t boxPos8 = DrawUtils::worldToScreen(vec3_t(ent->aabb.upper.x, ent->aabb.upper.y, ent->aabb.lower.z));

		DrawUtils::fillRectangle(vec4_t(boxPos1.x, boxPos1.y, boxPos2.x, boxPos2.y), MC_Color(200, 200, 200), 0.3f); //底面
		DrawUtils::fillRectangle(vec4_t(boxPos5.x, boxPos5.y, boxPos6.x, boxPos6.y), MC_Color(200, 200, 200), 0.3f); //顶面
		DrawUtils::fillRectangle(vec4_t(boxPos1.x, boxPos1.y, boxPos7.x, boxPos7.y), MC_Color(200, 200, 200), 0.3f);
		DrawUtils::fillRectangle(vec4_t(boxPos1.x, boxPos1.y, boxPos8.x, boxPos8.y), MC_Color(200, 200, 200), 0.3f);
		DrawUtils::fillRectangle(vec4_t(boxPos5.x, boxPos5.y, boxPos3.x, boxPos3.y), MC_Color(200, 200, 200), 0.3f);
		DrawUtils::fillRectangle(vec4_t(boxPos5.x, boxPos5.y, boxPos4.x, boxPos4.y), MC_Color(200, 200, 200), 0.3f); //四个侧面
		*/ //失败的3DFillBox
	}
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
