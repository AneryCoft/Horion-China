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
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr || !g_Data.canUseMoveKeys())
		return;

	Level* level = localPlayer->level;

	if (level->rayHitType != 0)
		return; //是否指向方块

	vec3_ti pointingBlock = level->block;

	//if (localPlayer->region->getBlock(pointingBlock)->toLegacy()->blockId != 0) {
	DrawUtils::setColor(red / 255.f, green / 255.f, blue / 255.f, opacity);
	DrawUtils::drawBox(pointingBlock.toVec3t(), pointingBlock.add(1).toVec3t(), lineWidth, outline);
	//}
}