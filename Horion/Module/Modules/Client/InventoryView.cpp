#include "InventoryView.h"

InventoryView::InventoryView() : IModule(0, Category::CLIENT, "Renders inventory on screen") {
	registerFloatSetting("ViewPosX", &invX, invX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("ViewPosY", &invY, invY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerIntSetting("Red", &red, red, 0, 255);
	registerIntSetting("Green", &green, green, 0, 255);
	registerIntSetting("Blue", &blue, blue, 0, 255);
	registerBoolSetting("Rainbow", &rainbow, rainbow);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
	registerFloatSetting("BackGround", &backGround, backGround, 0.f, 1.f);
}

InventoryView::~InventoryView() {
}

const char* InventoryView::getModuleName() {
	return ("InvView");
}

static float currColor[4];

void InventoryView::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())
		return;

	C_Inventory* inventory = g_Data.getLocalPlayer()->getSupplies()->inventory;

	float sideBars = 1.5f;
	float yVal = invY + sideBars;
	xVal = invX + sideBars;
	{
		if (currColor[3] < 1) {
			currColor[0] = 1;
			currColor[1] = 0.2f;
			currColor[2] = 0.2f;
			currColor[3] = 1;
		}
		Utils::ApplyRainbow(currColor, 0.0010f);
	}

	if (rainbow) {
		DrawUtils::setColor(currColor[1], currColor[2], currColor[3], opacity);
	}
	else {
		DrawUtils::setColor(red / 255.f, green / 255.f, blue / 255.f, opacity);
	}

	std::string text = "Inventory";
	DrawUtils::drawText(vec2_t(invX, invY - 10.f), &text, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity));

	DrawUtils::drawRectangle(vec2_t(invX, invY), vec2_t(invX + 152.f + sideBars, invY + 60.f + sideBars), sideBars / 2);

	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
	switch (ClientThemes->Theme.selected) {
	case 0:
		DrawUtils::fillRectangle(vec4_t(invX, invY, invX + 152.f + sideBars, invY + 60.f + sideBars), MC_Color(180, 180, 180), backGround);
		break;
	case 1:
		DrawUtils::fillRectangle(vec4_t(invX, invY, invX + 152.f + sideBars, invY + 60.f + sideBars), MC_Color(12, 12, 12), backGround);
		break;
	case 2:
		DrawUtils::fillRectangle(vec4_t(invX, invY, invX + 152.f + sideBars, invY + 60.f + sideBars), MC_Color(13, 29, 48), backGround);
	}

	for (int i = 9; i < 18; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal, yVal + 0.f), 1.f, 1.f - 0.20f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string stackStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 0.f) + 9.5f), &stackStr, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity), 0.8f);
		}
		xVal += 17.f;
	}
	xVal -= 180.f;
	for (int i = 18; i < 27; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal + 27.f, yVal + 20.f), 1.f, 1.f - 0.20f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string stackStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 33.5f, (yVal + 20.f) + 9.5f), &stackStr, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity), 0.8f);
		}
		xVal += 17.f;
	}
	xVal -= 180.f;
	for (int i = 27; i < 36; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal + 54.f, yVal + 40.f), 1.f, 1.f - 0.20f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string stackStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 60.5f, (yVal + 40.f) + 9.5f), &stackStr, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity), 0.8f);
		}
		xVal += 17.f;
	}
	xVal -= 180.f;
	/*
	for (int j = 0; j < 9; j++) {
		if ((item->getItemStack(j)->item != nullptr)) {
			DrawUtils::drawItem(item->getItemStack(j), vec2_t(xVal + 80.f, yVal + 60.f), 1.f, 1.f - .30f, false);
			std::string stackStr = std::to_string(item->getItemStack(j)->count);
			if (item->getItemStack(j)->count > 1)
				DrawUtils::drawText(vec2_t(xVal + 86.5f, (yVal + 60.f) + 9.5f), &stackStr, MC_Color(InventoryView::rtext, InventoryView::gtext, InventoryView::btext), scale);
		}
		xVal += 17.f;
	}
	xVal -= 180.f;
	*/
}