#include "TargetHud.h"

TargetHud::TargetHud() : IModule(0, Category::VISUAL, "Displays some information about the target") {
	//registerBoolSetting("Position", &displayPosition, displayPosition);
	//registerBoolSetting("Distance", &displayDistance, displayDistance);
	//registerBoolSetting("Health", &displayHealth, displayHealth);
	//registerBoolSetting("ArmorValue", &displayArmorValue, displayArmorValue);
}

TargetHud::~TargetHud() {
}

const char* TargetHud::getModuleName() {
	return ("TargetHud");
}

void TargetHud::onAttack(C_Entity* attackedEnt) {
	if (Target::isValidTarget(attackedEnt)) {
		target = attackedEnt;
		attacked = true;
		renderTime.resetTime();
	}
}

void TargetHud::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (attacked == true) {
		attacked = false;
	}
	else {
		if (renderTime.hasTimedElapsed(3000.f, true)) {
			target = nullptr;
		}
	}

	if (target != nullptr && Target::isValidTarget(target)) {
		std::string name = target->getNameTag()->getText();

		name = Utils::onlyOneLine(name);
		//name = std::regex_replace(name,std::regex("\n"), " "); //将换行改为空格
		std::string nameStr = "Name : " + name + "\n";
		//名字

		vec3_t position = *target->getPos();
		std::string posStr = "Position : " +
			std::to_string((int)floor(position.x)) + " " +
			std::to_string((int)floor(position.y)) + " " +
			std::to_string((int)floor(position.z)) + "\n";
		//位置

		float distance = g_Data.getLocalPlayer()->getPos()->dist(position);
		char disValueStr[16];
		sprintf_s(disValueStr, 16, "%.1f", distance);
		std::string disStr = "Distance : " + std::string(disValueStr) + "\n";
		//距离

		float maxHealth = target->getAttribute(&HealthAttribute())->maximumValue;
		float currentHealth = target->getAttribute(&HealthAttribute())->currentValue;

		std::string healthStr = "Health : "
			+ std::to_string((int)currentHealth) +
			"/" +
			std::to_string((int)maxHealth) +
			"\n";
		//生命

		float armorValue = 0.f;
		for (int i = 0; i < 4; i++) {
			C_ItemStack* stack = target->getArmor(i);

			if (stack->item != nullptr) {
				armorValue += (*stack->item)->getArmorValue();
			}
		}
		std::string armorStr = "ArmorValue : " + std::to_string((int)armorValue) + "/20" + "\n";
		//盔甲值

		//获取信息

		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

		float length = DrawUtils::getTextWidth(&std::string("Name : abcdefghijklmno"), 1.f);
		float currentLength = DrawUtils::getTextWidth(&nameStr, 1.f);
		if (currentLength > length)
			length = currentLength;

		vec4_t hud = vec4_t(windowSize.x / 1.6f - 5.f
			, windowSize.y / 2 + windowSize.y / 12.f - 5.f
			, windowSize.x / 1.6f + length + 5.f
			, windowSize.y / 2 + (windowSize.y + 455.f) / 12.f + 5.f);

		static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
		if (ClientThemes->Theme.selected == 0) {
			DrawUtils::fillRectangle(hud, MC_Color(12, 12, 12), 0.3f);
		}
		else {
			DrawUtils::fillRectangle(hud, MC_Color(13, 29, 48), 0.3f);
		}

		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + windowSize.y / 12.f),
			&nameStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + (windowSize.y + 85.f) / 12.f),
			&posStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + (windowSize.y + 170.f) / 12.f),
			&disStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		vec4_t healthBars = vec4_t(windowSize.x / 1.6f
			, windowSize.y / 2 + (windowSize.y + 275.f) / 12.f
			, (windowSize.x / 1.6f) + length * (currentHealth / maxHealth)
			, windowSize.y / 2 + (windowSize.y + 360.f) / 12.f);
		if (target->damageTime > 0)
			DrawUtils::fillRectangle(healthBars, MC_Color(255, 161, 161), 0.9f);
		else
			DrawUtils::fillRectangle(healthBars, MC_Color(255, 19, 19), 0.9f);

		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + (windowSize.y + 275.f) / 12.f),
			&healthStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		vec4_t armorBars = vec4_t(windowSize.x / 1.6f
			, windowSize.y / 2 + (windowSize.y + 370.f) / 12.f
			, (windowSize.x / 1.6f) + length * (armorValue / 20.f)
			, windowSize.y / 2 + (windowSize.y + 455.f) / 12.f);
		DrawUtils::fillRectangle(armorBars, MC_Color(74, 237, 217), 0.9f);
		
		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + (windowSize.y + 370.f) / 12.f),
			&armorStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		//渲染
	}
}
