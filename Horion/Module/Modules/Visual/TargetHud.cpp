#include "TargetHud.h"

TargetHud::TargetHud() : IModule(0, Category::VISUAL, "Displays some information about the target") {
	armor = SettingEnum(this)
		.addEntry(EnumEntry("ArmorValue", 0))
		.addEntry(EnumEntry("ArmorModel", 1));
	registerEnumSetting("ArmorDisplay", &armor, 0);
	/*registerBoolSetting("Position", &displayPosition, displayPosition);
	registerBoolSetting("Distance", &displayDistance, displayDistance);
	registerBoolSetting("Health", &displayHealth, displayHealth);
	registerBoolSetting("Armor", &displayArmor, displayArmor);*/
}

TargetHud::~TargetHud() {
}

const char* TargetHud::getModuleName() {
	return ("TargetHud");
}

void TargetHud::onAttack(C_Entity* attackedEnt) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	}

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

	if (Target::isValidTarget(target)) {
		std::string name = target->getNameTag()->getText();

		name = Utils::onlyOneLine(name);
		//name = std::regex_replace(name,std::regex("\n"), " "); //将换行改为空格
		if (name.empty())
			name = target->getType()->getText();
		std::string nameStr = "Name : " + name;
		//名字 

		vec3_t position = *target->getPos();
		std::string posStr = "Position : " +
			std::to_string((int)floor(position.x)) + " " +
			std::to_string((int)floor(position.y)) + " " +
			std::to_string((int)floor(position.z));
		//位置 

		float distance = g_Data.getLocalPlayer()->getPos()->dist(position);
		char disValueStr[16];
		sprintf_s(disValueStr, 16, "%.1f", distance);
		std::string disStr = "Distance : " + std::string(disValueStr);
		//距离 

		float maxHealth = target->getAttribute(&HealthAttribute())->maximumValue;
		float currentHealth = target->getAttribute(&HealthAttribute())->currentValue;

		std::string healthStr = "Health : "
			+ std::to_string((int)currentHealth) +
			"/" +
			std::to_string((int)maxHealth);
		//生命 

		float armorValue = 0.f;
		for (int i = 0; i < 4; i++) {
			C_ItemStack* stack = target->getArmor(i);

			if (stack->item != nullptr) {
				armorValue += (*stack->item)->getArmorValue();
			}
		}

		std::string armorStr = "ArmorValue : " + std::to_string((int)armorValue) + "/20";
		//盔甲值 

		//获取信息 

		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

		//float length = DrawUtils::getTextWidth(&std::string("Name : abcdefghijklmno"), 1.f);
		float length = 89.f;
		float currentLength = DrawUtils::getTextWidth(&nameStr, 1.f);
		if (currentLength > length)
			length = currentLength;

		float hight = windowSize.y;

		vec4_t hud = vec4_t(windowSize.x / 1.6f - 5.f
			, windowSize.y / 2 + windowSize.y / 12.f - 5.f
			, windowSize.x / 1.6f + length + 5.f
			, windowSize.y / 2 + (windowSize.y + 455.f) / 12.f + 5.f);

		static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
		switch (ClientThemes->Theme.selected) {
		case 0:
			DrawUtils::fillRectangle(hud, MC_Color(180, 180, 180), 0.4f);
			break;
		case 1:
			DrawUtils::fillRectangle(hud, MC_Color(12, 12, 12), 0.4f);
			break;
		case 2:
			DrawUtils::fillRectangle(hud, MC_Color(13, 29, 48), 0.4f);
		}

		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + hight / 12.f),
			&nameStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		hight += 85.f;
		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + hight / 12.f),
			&posStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		hight += 85.f;
		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + hight / 12.f),
			&disStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);

		hight += (85.f + 20.f);
		vec4_t healthBars = vec4_t(windowSize.x / 1.6f
			, windowSize.y / 2 + hight / 12.f
			, (windowSize.x / 1.6f) + length * (currentHealth / maxHealth)
			, windowSize.y / 2 + (hight + 85.f) / 12.f);
		if (target->damageTime > 0)
			DrawUtils::fillRectangle(healthBars, MC_Color(255, 161, 161), 0.9f);
		else
			DrawUtils::fillRectangle(healthBars, MC_Color(255, 19, 19), 0.9f);

		DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
			windowSize.y / 2 + hight / 12.f),
			&healthStr,
			MC_Color(1.f, 1.f, 1.f), 1.f);
		
		hight += (85.f + 10.f);
		if (armor.selected == 0) {
			vec4_t armorBars = vec4_t(windowSize.x / 1.6f
				, windowSize.y / 2 + hight / 12.f
				, (windowSize.x / 1.6f) + length * (armorValue / 20.f)
				, windowSize.y / 2 + (hight + 85.f) / 12.f);
			DrawUtils::fillRectangle(armorBars, MC_Color(74, 237, 217), 0.9f);

			DrawUtils::drawText(vec2_t(windowSize.x / 1.6f,
				windowSize.y / 2 + hight / 12.f),
				&armorStr,
				MC_Color(1.f, 1.f, 1.f), 1.f);
		} //显示盔甲值 
		else if (armor.selected == 1) {
			float x = 0.f;
			float itemPosY = windowSize.y / 2 + hight / 12.f;

			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = target->getArmor(i);
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(windowSize.x / 1.6f + x, itemPosY), 0.9f, 0.8f, stack->isEnchanted());
				}
				x += 12.f;
			}
			//盔甲 

			if (target->isPlayer()) {
				C_Player* player = reinterpret_cast<C_Player*>(target);
				C_ItemStack* stack = player->getSelectedItem();
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(windowSize.x / 1.6f + x, itemPosY), 0.9f, 0.8f, stack->isEnchanted());
				}
			}
			//手持物品 
		} //显示盔甲模型 

		//渲染 
	}
}
