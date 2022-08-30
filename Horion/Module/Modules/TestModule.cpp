#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/VoxelIntersector.h"
#include "../../DrawUtils.h"
#include "../../../SDK/MatrixStack.h"
#include <deque>
#include <array>
#include <unordered_set>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>			//radians
#include <glm/ext/matrix_transform.hpp>		// perspective, translate, rotate
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../../../Utils/Json.hpp"
#include "../../path/JoePathFinder.h"
#include "../../path/JoeMovementController.h"
#include "../../path/goals/JoeGoal.h"
#include "../../path/goals/JoeGoalXZ.h"

using json = nlohmann::json;


TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes only!") {
	enum1 = SettingEnum(this)
		.addEntry(EnumEntry("1", 1))
		.addEntry(EnumEntry("2", 2))
		.addEntry(EnumEntry("3", 3));
	
	registerBoolSetting("ItemInfo", &itemInfo, itemInfo);
	registerBoolSetting("EntityInfo", &entityInfo, entityInfo);
	registerFloatSetting("float1", &float1, 0, -10, 10);
	registerIntSetting("int1", &int1, 0, -10, 10);
	registerEnumSetting("Enum1", &enum1, 0);
	registerBoolSetting("bool1", &bool1, true);
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

void TestModule::onEnable() {
}

void TestModule::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) {
		return;
	}

	if (g_Data.getLocalPlayer()->getSelectedItem()->item != nullptr) {
		clientMessageF("ItemID=%i extraData=%i Damage = %i", (*g_Data.getLocalPlayer()->getSelectedItem()->item)->itemId, g_Data.getLocalPlayer()->getSelectedItem()->extraData, (*g_Data.getLocalPlayer()->getSelectedItem()->item)->getAttackDamage());
	}
	//clientMessageF("color=%i", g_Data.getLocalPlayer()->getArmorColorInSlot(0, 0));
}

void TestModule::onMove(C_MoveInputHandler* hand){
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TestModule::onSendPacket(C_Packet* packet, bool& cancelSend) {
}

void TestModule::onDisable() {
}

void TestModule::onLevelRender() {
}

void TestModule::onAttack(C_Entity* attackedEnt) {
	if (entityInfo) {
		logF("EntityTypeId=%i,NameTag=%s,Hight=%f,Width=%f", attackedEnt->getEntityTypeId(), attackedEnt->getNameTag()->getText(), attackedEnt->height, attackedEnt->width);
		logF("NameTag=%s", attackedEnt->getNameTag()->getText());
	}
}