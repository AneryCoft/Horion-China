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
	if (itemInfo) {
		if (g_Data.getLocalPlayer()->getSelectedItem()->item != nullptr) {
			clientMessageF("ItemID=%i extraData=%i Damage = %i", (*g_Data.getLocalPlayer()->getSelectedItem()->item)->itemId, g_Data.getLocalPlayer()->getSelectedItem()->extraData, (*g_Data.getLocalPlayer()->getSelectedItem()->item)->getAttackDamage());
		}
	}
	//clientMessageF("color=%i", g_Data.getLocalPlayer()->getArmorColorInSlot(0, 0));
}

void TestModule::onMove(C_MoveInputHandler* hand) {
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
		logF("EntityTypeId=%i,Height=%f,Width=%f,Hurttime=%i,Sneaking=%i,CanShowName=%i,NameTag=%s", attackedEnt->getEntityTypeId(), attackedEnt->height, attackedEnt->width, attackedEnt->damageTime, attackedEnt->isSneaking(), attackedEnt->canShowNameTag(), attackedEnt->getNameTag()->getText());
		logF("lower(%f,%f,%f) upper(%f,%f,%f)", attackedEnt->aabb.lower.x, attackedEnt->aabb.lower.y, attackedEnt->aabb.lower.z, attackedEnt->aabb.upper.x, attackedEnt->aabb.upper.y, attackedEnt->aabb.upper.z);
		logF("Pos(%f,%f,%f)", attackedEnt->getPos()->x, attackedEnt->getPos()->y, attackedEnt->getPos()->z);
	}
}