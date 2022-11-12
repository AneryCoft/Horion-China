#include "BlockOutline.h"

BlockOutline::BlockOutline() : IModule(0, Category::VISUAL, "Render the block you are pointing to.") {
	registerIntSetting("Red", &red, red, 0, 255);
	registerIntSetting("Green", &green, green, 0, 255);
	registerIntSetting("Blue", &blue, blue, 0, 255);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
	registerFloatSetting("lineWidth", &lineWidth, lineWidth, 0.1f, 1.f);
	registerBoolSetting("Outline", &outline, outline);
}

BlockOutline::~BlockOutline() {
}

const char* BlockOutline::getModuleName() {
	return ("BlockOutline");
}

void BlockOutline::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	Level* level = g_Data.getLocalPlayer()->level;

	if (g_Data.getLocalPlayer()->region->getBlock(level->block)->toLegacy()->blockId != 0) {
		DrawUtils::setColor(red / 255.f, green / 255.f, blue / 255.f, opacity);
		DrawUtils::drawBox(level->block.toVec3t(), level->block.add(1).toVec3t(), 0.5f, outline);
	}
}