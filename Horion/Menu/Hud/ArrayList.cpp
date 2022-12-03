#include "ArrayList.h"

void ArrayList::renderArrayList() {
	static auto hudModule = moduleMgr->getModule<HudModule>();
	static auto arrayListModule = moduleMgr->getModule<ArrayListModule>();
	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
	static auto fontChangerModule = moduleMgr->getModule<FontChanger>();

	static float rcolors[4];          // Rainbow color array RGBA
	static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
	static float currColor[4];        // ArrayList colors

	// Rainbow color updates
	{
		Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
		disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
		disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
		disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
		disabledRcolors[3] = 1;
	}

	// Display ArrayList on the Right?
	static constexpr bool isOnRightSide = true;

	float yOffset = 0;  // Offset of next Text
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	// Convert mousePos to visual Pos
	{
		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	// Convert mousePos to visual Pos

	// Parameters
	float textSize = hudModule->scale;
	float textPadding = 1.0f * textSize;
	float textHeight = 10.0f * textSize;
	float smoothness = 10;

	struct IModuleContainer {
		// Struct used to Sort IModules in a std::set
		std::shared_ptr<IModule> backingModule;
		std::string moduleName;
		bool enabled;
		int keybind;
		float textWidth;
		vec2_t* pos;
		bool shouldRender = true;

		IModuleContainer(std::shared_ptr<IModule> mod) {
			const char* moduleNameChr = mod->getModuleName();
			this->enabled = mod->isEnabled();
			this->keybind = mod->getKeybind();
			this->backingModule = mod;
			this->pos = mod->getPos();

			if (keybind == 0x0)
				moduleName = moduleNameChr;
			else {
				char text[50];
				sprintf_s(text, 50, "%s%s", moduleNameChr, arrayListModule->keybinds ? std::string(" [" + std::string(Utils::getKeybindName(keybind)) + "]").c_str() : "");
				moduleName = text;
			}

			if (!this->enabled && *this->pos == vec2_t(0.f, 0.f))
				this->shouldRender = false;
			this->textWidth = DrawUtils::getTextWidth(&moduleName, hudModule->scale);
		}

		bool operator<(const IModuleContainer& other) const {
			/*if (enabled) {
		if (!other.enabled)  // We are enabled
			return true;
	} else if (other.enabled)  // They are enabled
		return false;*/

			if (this->textWidth == other.textWidth)
				return moduleName < other.moduleName;
			return this->textWidth > other.textWidth;
		}
	};

	// Mouse click detector
	static bool wasLeftMouseDown = GameData::isLeftClickDown();  // Last isDown value
	bool leftMouseDown = GameData::isLeftClickDown();            // current isDown value

	bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown;  // isDown == true AND (current state IS NOT last state)
	wasLeftMouseDown = leftMouseDown;                                        // Set last isDown value

	std::set<IModuleContainer> modContainerList;
	// Fill modContainerList with Modules
	{
		auto lock = moduleMgr->lockModuleList();
		std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
		for (auto it : *moduleList) {
			if (it.get() != hudModule && it.get() != arrayListModule && it.get() != ClientThemes && it.get() != fontChangerModule)
				modContainerList.emplace(IModuleContainer(it));
		}
	}

	int a = 0;
	int b = 0;
	int c = 0;

	// Loop through mods to display Labels
	for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {
		if (!it->shouldRender)
			continue;

		std::string textStr = it->moduleName;
		float textWidth = it->textWidth;

		float xOffsetOri = windowSize.x - textWidth - (textPadding * 2);

		float xOffset = windowSize.x - it->pos->x;

		it->pos->x += smoothness;

		if (xOffset < xOffsetOri) {
			xOffset = xOffsetOri;
		}
		if (!it->enabled) {
			xOffset += it->pos->y;
			it->pos->y += smoothness;
		}
		if (xOffset >= windowSize.x && !it->enabled) {
			it->pos->x = 0.f;
			it->pos->y = 0.f;
		}

		vec2_t textPos = vec2_t(
			xOffset + textPadding,
			yOffset + textPadding);
		vec4_t rectPos = vec4_t(
			xOffset - 2,
			yOffset,
			isOnRightSide ? windowSize.x : textWidth + (textPadding * 2),
			yOffset + textPadding * 2 + textHeight);
		vec4_t leftRect = vec4_t(
			xOffset - 2,
			yOffset,
			xOffset - 1,
			yOffset + textPadding * 2 + textHeight);
		/*vec4_t downRect = vec4_t(
			xOffset - 1,
			yOffset + textPadding * 2 + textHeight - 1,
			xOffset + textPadding * 2 + textWidth,
			yOffset + textPadding * 2 + textHeight);*/
		c++;
		b++;
		if (b < 20)
			a = moduleMgr->getEnabledModuleCount() * 2;
		else
			b = 0;
		currColor[3] = rcolors[3];
		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
		currColor[0] += 1.f / a * c;
		Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

		if (arrayListModule->fill) {
			switch (ClientThemes->Theme.selected) {
			case 0:
				DrawUtils::fillRectangle(rectPos, MC_Color(180, 180, 180), 0.5f);
				break;
			case 1:
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
				break;
			case 2:
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
		}

		if (arrayListModule->border) {
			DrawUtils::fillRectangle(leftRect, MC_Color(currColor), 0.8f);
		}
		//DrawUtils::fillRectangle(downRect, MC_Color(currColor), 0.8f);
		if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && arrayListModule->clickToggle) {
			vec4_t selectedRect = rectPos;
			selectedRect.x = leftRect.z;
			if (leftMouseDown) {
				if (arrayListModule->fill)
					DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
				if (executeClick)
					it->backingModule->toggle();
			}
			else
				if (arrayListModule->fill)
					DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
		}
		DrawUtils::drawText(textPos, &textStr, MC_Color(currColor), textSize);

		yOffset += textHeight + (textPadding * 2);
	}
	c = 0;
	modContainerList.clear();
}