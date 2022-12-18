#include "Tracer.h"

Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities.") {
	registerIntSetting("Red", &red, red, 0, 255);
	registerIntSetting("Green", &green, green, 0, 255);
	registerIntSetting("Blue", &blue, blue, 0, 255);
	//registerBoolSetting("3D Tracers", &old, old);
}

Tracer::~Tracer() {
}

const char* Tracer::getModuleName() {
	return "Tracer";
}

/*
void Tracer::onLevelRender() {
		vec3_t origin = g_Data.getLocalPlayer()->level->rayHitVec;
		g_Data.forEachEntity([&](C_Entity* ent, bool valid) {
			if (Target::isValidTarget(ent) && g_Data.canUseMoveKeys()) {
				DrawUtils::setColor(255, 255, 255, 1);
				DrawUtils::drawLine3d(origin, *ent->getPos());
			}
		});
}
*/

std::shared_ptr<glmatrixf> refdef2;
void Tracer::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();
	refdef2 = std::shared_ptr<glmatrixf>(badrefdef->correct());

	g_Data.forEachEntity([&](C_Entity* ent, bool valid) {
		if (Target::isValidTarget(ent) && g_Data.canUseMoveKeys()) {
			vec2_t target;
			vec2_t screenSize(g_Data.getGuiData()->widthGame, g_Data.getGuiData()->heightGame);
			refdef2->OWorldToScreen(g_Data.getClientInstance()->levelRenderer->getOrigin(), *ent->getPos(), target, g_Data.getClientInstance()->getFov(), screenSize);
			vec2_t mid((screenSize.x / 2), (screenSize.y / 2));
			if (target != vec2_t(0, 0)) {
				DrawUtils::setColor(red / 255.f, green / 255.f, blue / 255.f, 1);
				DrawUtils::drawLine(mid, target, 0.2f);
			}
		}
		});
}