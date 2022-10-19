#include"Watermark.h"

void Watermark::renderWatermark() {
	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();

	static float rcolors[4];          // Rainbow color array RGBA
	static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules

	// Rainbow color updates
	{
		Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
		disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
		disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
		disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
		disabledRcolors[3] = 1;
	}

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

	constexpr float nameTextSize = 1.5f;
	constexpr float versionTextSize = 0.7f;
	static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
	constexpr float borderPadding = 1;
	constexpr float margin = 5;

	static std::string name = "Horion";
#ifdef _DEBUG
	static std::string version = "dev";
#elif defined _BETA
	static std::string version = "beta";
#else
	static std::string version = "China";
#endif

	float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
	float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize);
	vec4_t rect = vec4_t(
		windowSize.x - margin - fullTextLength - borderPadding * 2,
		windowSize.y - margin - textHeight,
		windowSize.x - margin + borderPadding,
		windowSize.y - margin);

	DrawUtils::drawRectangle(rect, MC_Color(rcolors), 1.f, 2.f);
	if (ClientThemes->Theme.selected == 1) {
		DrawUtils::fillRectangle(rect, MC_Color(13, 29, 48), 1.f);
	}
	else {
		DrawUtils::fillRectangle(rect, MC_Color(12, 12, 12), 1.f);
	}
	DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(rcolors), nameTextSize);
	DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.w - 7), &version, MC_Color(rcolors), versionTextSize);
}