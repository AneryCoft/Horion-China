#include "Breadcrumbs.h"

Breadcrumbs::Breadcrumbs() : IModule(0, Category::VISUAL, "Draw the route you've taken") {
}

Breadcrumbs::~Breadcrumbs() {
}

const char* Breadcrumbs::getModuleName() {
	return ("Breadcrumbs");
}

void Breadcrumbs::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	vec3_t floorPos = *g_Data.getLocalPlayer()->getPos();
	floorPos.y = g_Data.getLocalPlayer()->getAABB()->lower.y + 0.1f;

	points.push_back(floorPos);

	if (!g_Data.canUseMoveKeys())
		return;

	if (!points.empty()) {
		DrawUtils::drawText(DrawUtils::worldToScreen(points[0]), &std::string("O"), MC_Color(28 / 255.0f, 107 / 255.0f, 201 / 255.0f, 1.f));

		DrawUtils::setColor(28 / 255.0f, 107 / 255.0f, 201 / 255.0f, 1.f);
		DrawUtils::drawLinestrip3d(points);
	}
}

void Breadcrumbs::onDisable() {
	points.clear();
}
