#include "InventoryView.h"

InventoryView::InventoryView() : IModule(0, Category::CLIENT, "renders inventory on screen") {
	registerFloatSetting("ViewPosX", &invX, invX, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.x);
	registerFloatSetting("ViewPosY", &invY, invY, 0.f, g_Data.getClientInstance()->getGuiData()->windowSize.y);
	registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
	registerFloatSetting("Red", &rtext, rtext, 0.f, 1.f);
	registerFloatSetting("Green", &gtext, gtext, 0.f, 1.f);
	registerFloatSetting("Blue", &btext, btext, 0.f, 1.f);
	registerBoolSetting("RGB", &RGB, RGB);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
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
		Utils::ApplyRainbow(currColor, 0.00025f);
	}

	if (RGB) {
		DrawUtils::setColor(currColor[1], currColor[2], currColor[3], opacity);
	}
	else {
		DrawUtils::setColor(255, 255, 255, opacity);
	}

	std::string text = "Inventory";
	DrawUtils::drawText(vec2_t(invX, invY - 10.f), &text, MC_Color(255, 255, 255), 1.f);

	DrawUtils::drawRectangle(vec2_t(invX, invY), vec2_t(invX + 152.f + sideBars, invY + 60.f + sideBars), sideBars / 2);

	for (int i = 9; i < 18; i++) {
		C_ItemStack* stack = inventory->getItemStack(i);
		if (stack->item != nullptr) {
			DrawUtils::drawItem(stack, vec2_t(xVal, yVal + 0.f), 1.f, 1.f - 0.20f, stack->isEnchanted());
			char itemCount = stack->count;
			std::string stackStr = std::to_string(itemCount);
			if (itemCount > 1)
				DrawUtils::drawText(vec2_t(xVal + 6.5f, (yVal + 0.f) + 9.5f), &stackStr, MC_Color(InventoryView::rtext, InventoryView::gtext, InventoryView::btext), scale);
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
				DrawUtils::drawText(vec2_t(xVal + 33.5f, (yVal + 20.f) + 9.5f), &stackStr, MC_Color(InventoryView::rtext, InventoryView::gtext, InventoryView::btext), scale);
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
				DrawUtils::drawText(vec2_t(xVal + 60.5f, (yVal + 40.f) + 9.5f), &stackStr, MC_Color(InventoryView::rtext, InventoryView::gtext, InventoryView::btext), scale);
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