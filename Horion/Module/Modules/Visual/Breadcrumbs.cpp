#include "Breadcrumbs.h"

Breadcrumbs::Breadcrumbs() : IModule(0, Category::VISUAL, "Draw the route you've taken") {
	registerIntSetting("Red", &red, red, 0, 255);
	registerIntSetting("Green", &green, green, 0, 255);
	registerIntSetting("Blue", &blue, blue, 0, 255);
	registerBoolSetting("Rainbow", &rainbow, rainbow);
}

Breadcrumbs::~Breadcrumbs() {
}

const char* Breadcrumbs::getModuleName() {
	return ("Breadcrumbs");
}

static float rainbowColor[4];

void Breadcrumbs::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (!points.empty()) {
		MC_Color color;
		if (rainbow) {
			color = MC_Color(rainbowColor[0], rainbowColor[1], rainbowColor[2], 1.f);
		}
		else {
			color = MC_Color(red / 255.f, green / 255.f, blue / 255.f, 1.f);
		}
		DrawUtils::drawText(DrawUtils::worldToScreen(points[0]), &std::string("O"), color);
	}
}

void Breadcrumbs::onLevelRender() {
	auto localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	vec3_t floorPos = *localPlayer->getPos();
	floorPos.y = localPlayer->getAABB()->lower.y + 0.1f;

	points.push_back(floorPos);

	if (!g_Data.canUseMoveKeys())
		return;

	if (!points.empty()) {
		if (rainbow) {
			if (rainbowColor[3] < 1) {
				rainbowColor[0] = 1;
				rainbowColor[1] = 0.2f;
				rainbowColor[2] = 0.2f;
				rainbowColor[3] = 1;
			}
			Utils::ApplyRainbow(rainbowColor, 0.001f);
			DrawUtils::setColor(rainbowColor[0], rainbowColor[1], rainbowColor[2], 1.f);
		}
		else {
			DrawUtils::setColor(red / 255.f, green / 255.f, blue / 255.f, 1.f);
		}
		DrawUtils::drawLinestrip3d(points);
	}
}

void Breadcrumbs::onDisable() {
	points.clear();
}
