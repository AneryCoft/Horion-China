#include "TargetHud.h"

TargetHud::TargetHud() : IModule(0, Category::VISUAL, "Displays some information about the target") {
	registerBoolSetting("Position", &displayPosition, displayPosition);
	registerBoolSetting("Distance", &displayDistance, displayDistance);
	registerBoolSetting("Health", &displayHealth, displayHealth);
	registerBoolSetting("ArmorValue", &displayArmorValue, displayArmorValue);
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

void TargetHud::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
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

	if (target != nullptr && target->isEntityExist()) {
		std::string name = target->getNameTag()->getText();
		name = std::regex_replace(name,std::regex("\n"), " "); //将换行改为空格
		std::string nameStr = "Name : " + name + "\n"; //名字

		vec3_t position = *target->getPos();
		std::string posStr = "Position : " +
			std::to_string((int)floor(position.x)) + " " +
			std::to_string((int)floor(position.y)) + " " +
			std::to_string((int)floor(position.z)) + "\n"; //位置

		float distance = g_Data.getLocalPlayer()->getPos()->dist(position);
		char str[16];
		sprintf_s(str, 16, "%.1f", distance);
		std::string disStr = "Distance : " + std::string(str) + "\n"; //距离

		float maxHealth = target->getAttribute(&HealthAttribute())->maximumValue;
		float currentHealth = target->getAttribute(&HealthAttribute())->currentValue;
		std::string healthStr = "Health : "
			+ std::to_string((int)currentHealth) +
			"/" +
			std::to_string((int)maxHealth) +
			"\n"; //生命

		float armorValue = 0.f;
		for (int i = 0; i < 4; i++) {
			C_ItemStack* stack = target->getArmor(i);

			if (stack->item != nullptr) {
				armorValue += (*stack->item)->getArmorValue();
			}
		}
		std::string armorStr = "ArmorValue : " + std::to_string((int)armorValue) + "\n"; //盔甲值


		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

		vec4_t hud = vec4_t(windowSize.x / 2.f + 30.f, windowSize.y / 2.f + 20.f, windowSize.x / 2.f + 80.f, windowSize.y / 2.f + 20.f);
		//DrawUtils::fillRectangle(hud, MC_Color(0.05f, 0.05f, 0.05f), 0.35f);
		//DrawUtils::drawText()


		DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
			windowSize.y / 2 - windowSize.y / 5.0f + windowSize.y / 4),
			&nameStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);
		DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
			windowSize.y / 2 - windowSize.y / 6.0f + windowSize.y / 4),
			&posStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);
		DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
			windowSize.y / 2 - windowSize.y / 7.0f + windowSize.y / 4),
			&disStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);
		DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
			windowSize.y / 2 - windowSize.y / 8.0f + windowSize.y / 4),
			&healthStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);
		DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
			windowSize.y / 2 - windowSize.y / 9.0f + windowSize.y / 4),
			&armorStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);
		//Test
	}
}