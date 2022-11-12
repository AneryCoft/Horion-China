#include "HudModule.h"
#include "../../../DrawUtils.h"
#include "../../../Scripting/ScriptManager.h"
#include "../../../Menu/Hud/TabGui.h"
#include "../../../Menu/Hud/ArrayList.h"
#include "../../../Menu/Hud/Notifications.h"
#include "../../../Menu/Hud/Watermark.h"

HudModule::HudModule() : IModule(0, Category::CLIENT, "Displays things like the ArrayList/TabGUI.") {
	registerBoolSetting("TabGui", &tabgui, tabgui);
	registerBoolSetting("ArrayList", &arraylist, arraylist);
	registerBoolSetting("ClickToggle", &clickToggle, clickToggle);
	registerBoolSetting("Notifications", &notifications, notifications);
	registerBoolSetting("Watermark", &watermark, watermark);
	registerBoolSetting("Coordinates", &coordinates, coordinates);
	registerBoolSetting("Show Keybinds", &keybinds, keybinds);
	registerBoolSetting("Show ArmorHUD", &displayArmor, displayArmor);
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
		if (key == *input->spaceBarKey) keyString = "-";
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 64.f,
			pos.y + 15.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 7.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		if (ClientThemes->Theme.selected == 1) {
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isKeyDown(key) ? 0.9f : 0.5f);
		}
		else {
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isKeyDown(key) ? 0.9f : 0.5f);
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

		if (ClientThemes->Theme.selected == 1) {
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isKeyDown(key) ? 0.9f : 0.5f);
		}
		else {
			DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isKeyDown(key) ? 0.9f : 0.5f);
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
	if (ClientThemes->Theme.selected == 1) {
		DrawUtils::fillRectangle(rectPos, GameData::isLeftClickDown() ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isLeftClickDown() ? 0.9f : 0.5f);
	}
	else {
		DrawUtils::fillRectangle(rectPos, GameData::isLeftClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isLeftClickDown() ? 0.9f : 0.5f);
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
	if (ClientThemes->Theme.selected == 1) {
		DrawUtils::fillRectangle(rectPos, GameData::isRightClickDown() ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), GameData::isRightClickDown() ? 0.9f : 0.5f);
	}
	else {
		DrawUtils::fillRectangle(rectPos, GameData::isRightClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), GameData::isRightClickDown() ? 0.9f : 0.5f);
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

	if (g_Data.getLocalPlayer() != nullptr) {
		// FPS
		if (fps) {
			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			if (ClientThemes->Theme.selected == 1) {
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			else {
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
			}
			DrawUtils::drawText(textPos, &fpsText, MC_Color(200, 200, 200), scale);

			startY += f;
		}

		// CPS
		if (cps) {
			std::string cpsText = "CPS: " + std::to_string(g_Data.getLeftCPS()) + " - " + std::to_string(g_Data.getRightCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			if (ClientThemes->Theme.selected == 1) {
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			else {
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
			}
			DrawUtils::drawText(textPos, &cpsText, MC_Color(200, 200, 200), scale);

			startY += f;
		}

		// Speed
		if (speed) {
			char str[16];
			sprintf_s(str, 16, "%.1f", g_Data.getLocalPlayer()->getBlocksPerSecond()); //仅保留一位小数
			std::string speedText = "Speed: " + std::string(str);

			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			if (ClientThemes->Theme.selected == 1) {
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			else {
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
			}
			DrawUtils::drawText(textPos, &speedText, MC_Color(200, 200, 200), scale);

			startY += f;
		}

		// Angle
#ifdef _DEBUG
		if (angle) {
			std::string coordsPitch = "Pitch: " + std::to_string((int)floorf(g_Data.getLocalPlayer()->pitch));
			std::string coordsYaw = "Yaw: " + std::to_string((int)floorf(g_Data.getLocalPlayer()->yaw));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 25.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			if (ClientThemes->Theme.selected == 1) {
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			else {
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
			}
			DrawUtils::drawText(textPos, &coordsPitch, MC_Color(200, 200, 200), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsYaw, MC_Color(200, 200, 200), scale);
			textPos.y += f;
			startY += 2 * f;
		}
#endif

		// Coordinates
		if (coordinates) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();

			std::string coordsX = "X: " + std::to_string((int)floorf(pos->x));
			std::string coordsY = "Y: " + std::to_string((int)floorf(pos->y));
			std::string coordsZ = "Z: " + std::to_string((int)floorf(pos->z));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			if (ClientThemes->Theme.selected == 1) {
				DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
			}
			else {
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 0.5f);
			}
			DrawUtils::drawText(textPos, &coordsX, MC_Color(200, 200, 200), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsY, MC_Color(200, 200, 200), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsZ, MC_Color(200, 200, 200), scale);
		}

		// ArmorHUD
		if (displayArmor && GameData::canUseMoveKeys()) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 15.f;
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			float x = windowSize.x / 2.f + 5.f;
			float y = windowSize.y - 57.5f;
			float y1 = windowSize.y - 63.5f;

			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->isValid()) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					C_Item* item = stack->getItem();
					if (item->getMaxDamage() > 0) {
						//std::string damageText = std::to_string(item->getMaxDamage() - item->getDamageValue(stack->tag)) + "/" + std::to_string(item->getMaxDamage()); //精准显示 但是文字会互相遮挡
						int damage = ((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage()) * 100;
						std::string damageText = std::to_string(damage) + "%";
						DrawUtils::drawText(vec2_t(x + scale, y1), &damageText, MC_Color(255, 255, 255), 0.8f, 1.f); //盔甲耐久度
					}
					x += scale * spacing;
				}
			}
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* stack = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			//x += scale * spacing;
			if (stack->isValid()) {
				C_Item* item = stack->getItem();

				if (item->getMaxDamage() > 0) {
					//std::string damageText2 = std::to_string(item->getItem()->getMaxDamage() - item->getItem()->getDamageValue(item->tag)) + "/" + std::to_string(item->getItem()->getMaxDamage());
					int damage2 = ((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage()) * 100;
					std::string damageText2 = std::to_string(damage2) + "%";
					DrawUtils::drawText(vec2_t(x + scale, y1), &damageText2, MC_Color(255, 255, 255), 0.8f, 1.f); //手持物品耐久度
				}
				DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
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

	if (moduleMgr->isInitialized()) {
		if (tabgui) {
			TabGui::renderTabGui();
		}
		if (arraylist) {
			ArrayList::renderArrayList();
		}
		if (notifications) {
			Notifications::renderNotifications();
		}
		if (watermark) {
			Watermark::renderWatermark();
		}
	}
}