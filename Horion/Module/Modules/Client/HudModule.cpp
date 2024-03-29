#include "HudModule.h"

HudModule::HudModule() : IModule(0, Category::CLIENT, "Displays things like the ArrayList/TabGUI.") {
	registerBoolSetting("TabGui", &tabgui, tabgui);
	registerBoolSetting("ArrayList", &arraylist, arraylist);
	//registerBoolSetting("Notifications", &notifications, notifications);
	registerBoolSetting("Watermark", &watermark, watermark);
	registerBoolSetting("Coordinates", &coordinates, coordinates);
	registerBoolSetting("Show Armor", &displayArmor, displayArmor);
	registerBoolSetting("Keystrokes", &keystrokes, keystrokes);
	registerBoolSetting("Show FPS", &fps, fps);
	registerBoolSetting("Show CPS", &cps, cps);
#ifdef _DEBUG
	registerBoolSetting("Show Angle", &angle, angle);
#endif
	registerBoolSetting("Show Speed", &speed, speed);
	registerBoolSetting("Always show", &alwaysShow, alwaysShow);
	registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::drawKeystroke(char key, const vec2_t& pos) {
	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
	std::string keyString = Utils::getKeybindName(key);
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	/*if (key == *input->forwardKey) {
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 20.f,
			pos.y + 20.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f) + 0.2f,
			rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
		if (ClientThemes->Theme.selected == 1) {
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), 0.5f);
		}
		else {
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 0.5f);
		}
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}*/
	if (key == *input->spaceBarKey) {
		if (key == *input->spaceBarKey) keyString = "SPACE";
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 64.f,
			pos.y + 15.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 7.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		switch (ClientThemes->Theme.selected) {
		case 0:
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(180, 180, 180) : MC_Color(120, 120, 120), GameData::isKeyDown(key) ? 0.9f : 0.5f);
			break;
		case 1:
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isKeyDown(key) ? 0.9f : 0.5f);
			break;
		case 2:
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isKeyDown(key) ? 0.9f : 0.5f);
		}

		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
	else {
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + ((key == *input->spaceBarKey) ? 64.f : 20.f),
			pos.y + ((key == *input->spaceBarKey) ? 15.f : 20.f));
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		switch (ClientThemes->Theme.selected) {
		case 0:
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(180, 180, 180) : MC_Color(120, 120, 120), GameData::isKeyDown(key) ? 0.9f : 0.5f);
			break;
		case 1:
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isKeyDown(key) ? 0.9f : 0.5f);
			break;
		case 2:
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isKeyDown(key) ? 0.9f : 0.5f);
		}
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
}

void HudModule::drawLeftMouseKeystroke(vec2_t pos) {
	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
	std::string keyString;
	keyString = "LMB";
	vec4_t rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	switch (ClientThemes->Theme.selected) {
	case 0:
		DrawUtils::fillRectangle(rectPos, GameData::isLeftClickDown() ? MC_Color(180, 180, 180) : MC_Color(120, 120, 120), GameData::isLeftClickDown() ? 0.9f : 0.5f);
		break;
	case 1:
		DrawUtils::fillRectangle(rectPos, GameData::isLeftClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isLeftClickDown() ? 0.9f : 0.5f);
		break;
	case 2:
		DrawUtils::fillRectangle(rectPos, GameData::isLeftClickDown() ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isLeftClickDown() ? 0.9f : 0.5f);
		break;
	}
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void HudModule::drawRightMouseKeystroke(vec2_t pos) {
	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
	std::string keyString;
	keyString = "RMB";
	vec4_t rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	switch (ClientThemes->Theme.selected) {
	case 0:
		DrawUtils::fillRectangle(rectPos, GameData::isRightClickDown() ? MC_Color(180, 180, 180) : MC_Color(120, 120, 120), GameData::isRightClickDown() ? 0.9f : 0.5f);
		break;
	case 1:
		DrawUtils::fillRectangle(rectPos, GameData::isRightClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isRightClickDown() ? 0.9f : 0.5f);
		break;
	case 2:
		DrawUtils::fillRectangle(rectPos, GameData::isRightClickDown() ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isRightClickDown() ? 0.9f : 0.5f);
		break;
	}
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void HudModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	if (tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer != nullptr) {
		// FPS
		if (fps) {
			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			switch (ClientThemes->Theme.selected) {
			case 0:
				DrawUtils::fillRectangle(rectPos, MC_Color(180, 180, 180), 0.35f);
				break;
			case 1:
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
				break;
			case 2:
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 255, 255), scale);

			startY += f;
		}

		// CPS
		if (cps) {
			std::string cpsText = "CPS: " + std::to_string(g_Data.getLeftCPS()) + " - " + std::to_string(g_Data.getRightCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			switch (ClientThemes->Theme.selected) {
			case 0:
				DrawUtils::fillRectangle(rectPos, MC_Color(180, 180, 180), 0.35f);
				break;
			case 1:
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
				break;
			case 2:
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			DrawUtils::drawText(textPos, &cpsText, MC_Color(255, 255, 255), scale);

			startY += f;
		}

		// Speed
		if (speed) {
			char str[16];
			sprintf_s(str, 16, "%.1f", localPlayer->getBlocksPerSecond()); //仅保留一位小数
			std::string speedText = "Speed: " + std::string(str);

			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			switch (ClientThemes->Theme.selected) {
			case 0:
				DrawUtils::fillRectangle(rectPos, MC_Color(180, 180, 180), 0.35f);
				break;
			case 1:
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
				break;
			case 2:
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			DrawUtils::drawText(textPos, &speedText, MC_Color(255, 255, 255), scale);

			startY += f;
		}

		// Angle
#ifdef _DEBUG
		if (angle) {
			std::string coordsPitch = "Pitch: " + std::to_string((int)floorf(localPlayer->pitch));
			std::string coordsYaw = "Yaw: " + std::to_string((int)floorf(localPlayer->yaw));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 25.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			switch (ClientThemes->Theme.selected) {
			case 0:
				DrawUtils::fillRectangle(rectPos, MC_Color(180, 180, 180), 0.35f);
				break;
			case 1:
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
				break;
			case 2:
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			DrawUtils::drawText(textPos, &coordsPitch, MC_Color(255, 255, 255), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsYaw, MC_Color(255, 255, 255), scale);
			textPos.y += f;
			startY += 2 * f;
		}
#endif

		// Coordinates
		if (coordinates) {
			vec3_t* pos = localPlayer->getPos();

			std::string coordsX = "X: " + std::to_string((int)floorf(pos->x));
			std::string coordsY = "Y: " + std::to_string((int)floorf(pos->y));
			std::string coordsZ = "Z: " + std::to_string((int)floorf(pos->z));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			switch (ClientThemes->Theme.selected) {
			case 0:
				DrawUtils::fillRectangle(rectPos, MC_Color(180, 180, 180), 0.35f);
				break;
			case 1:
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
				break;
			case 2:
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			DrawUtils::drawText(textPos, &coordsX, MC_Color(255, 255, 255), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsY, MC_Color(255, 255, 255), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsZ, MC_Color(255, 255, 255), scale);
		}

		// ArmorHUD
		if (displayArmor && GameData::canUseMoveKeys()) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 15.f;
			float x = windowSize.x / 2.f + 5.f;
			float y = windowSize.y - 57.5f;
			float y1 = windowSize.y - 63.5f;

			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = localPlayer->getArmor(i);
				if (stack->isValid()) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					C_Item* item = stack->getItem();
					if (item->getMaxDamage() > 0) {
						int armorDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
						std::string armorDamageText = std::to_string(armorDamage) + "%";
						int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
						DrawUtils::drawText(vec2_t(x + scale, y1), &armorDamageText, MC_Color(255 - i, i, 0), 0.8f, 1.f);
					} //盔甲耐久度
				}
				x += scale * spacing;
			}
			C_ItemStack* stack = localPlayer->getSelectedItem();
			//x += scale * spacing;
			if (stack->isValid()) {
				DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
				C_Item* item = stack->getItem();
				if (item->getMaxDamage() > 0) {
					int itemDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
					std::string itemDamageText = std::to_string(itemDamage) + "%";
					int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
					DrawUtils::drawText(vec2_t(x + scale, y1), &itemDamageText, MC_Color(255 - i, i, 0), 0.8f, 1.f);
				} //手持物品耐久度
			}
		}

		// Keystrokes
		if (keystrokes && GameData::canUseMoveKeys()) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			HudModule::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 84));
			HudModule::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 40));
			HudModule::drawLeftMouseKeystroke(vec2_t(10.f, windowSize.y - 25));
			HudModule::drawRightMouseKeystroke(vec2_t(43.f, windowSize.y - 25));
		}
	}

	static auto arrayListModule = moduleMgr->getModule<ArrayListModule>();
	static bool isEnableArray = false;

	if (arrayListModule->isEnabled() == arraylist)
		isEnableArray = arraylist;
	else {
		arrayListModule->setEnabled(!isEnableArray);
		arraylist = !isEnableArray;
	}

	if (moduleMgr->isInitialized()) {
		if (tabgui) {
			TabGui::renderTabGui();
		}
		if (watermark) {
			Watermark::renderWatermark();
		}
		if (arraylist) {
			ArrayList::renderArrayList();
		}
		if (notifications) {
			//Notifications::renderNotifications();
		}
	}
}