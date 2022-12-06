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

	vec4_t viewSize = vec4_t(invX, invY, invX + sideBars + 135.f, invY + sideBars + 45.f);

	DrawUtils::drawRectangle(vec2_t(viewSize.x, viewSize.y), vec2_t(viewSize.z, viewSize.w), sideBars / 2);

	static auto ClientThemes = moduleMgr->getModule<ClientTheme>();
	switch (ClientThemes->Theme.selected) {
	case 0:
		DrawUtils::fillRectangle(viewSize, MC_Color(180, 180, 180), backGround);
		break;
	case 1:
		DrawUtils::fillRectangle(viewSize, MC_Color(12, 12, 12), backGround);
		break;
	case 2:
		DrawUtils::fillRectangle(viewSize, MC_Color(13, 29, 48), backGround);
	}

	C_Inventory* inventory = g_Data.getLocalPlayer()->getSupplies()->inventory;

	for (int i = 9; i < 18; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal, yVal + 0.f), 1.f, 0.8f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string countStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 0.f) + 9.5f), &countStr, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity), 0.8f);
		}
		xVal += 15.f;
	}
	xVal -= 135.f;

	for (int i = 18; i < 27; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal, yVal + 15.f), 1.f, 0.8f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string countStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 15.f) + 9.5f), &countStr, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity), 0.8f);
		}
		xVal += 15.f;
	}
	xVal -= 135.f;

	for (int i = 27; i < 36; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal, yVal + 30.f), 1.f, 0.8f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string countStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 30.f) + 9.5f), &countStr, MC_Color(red / 255.f, green / 255.f, blue / 255.f, opacity), 0.8f);
		}
		xVal += 15.f;
	}
}