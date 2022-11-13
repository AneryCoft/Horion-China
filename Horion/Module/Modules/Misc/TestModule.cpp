#include "TestModule.h"
#include "../../../../Utils/Logger.h"
#include "../../../../Utils/VoxelIntersector.h"
#include "../../../DrawUtils.h"
#include "../../../../SDK/MatrixStack.h"
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
#include "../../../../Utils/Json.hpp"
#include "../../../path/JoePathFinder.h"
#include "../../../path/JoeMovementController.h"
#include "../../../path/goals/JoeGoal.h"
#include "../../../path/goals/JoeGoalXZ.h"

using json = nlohmann::json;


TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes only!") {
	enum1 = SettingEnum(this)
		.addEntry(EnumEntry("1", 1))
		.addEntry(EnumEntry("2", 2))
		.addEntry(EnumEntry("3", 3));

	registerBoolSetting("ItemInfo", &itemInfo, itemInfo);
	registerBoolSetting("EntityInfo", &entityInfo, entityInfo);
	registerBoolSetting("LocalInfo", &localInfo, localInfo);
	registerBoolSetting("BlockInfo", &blockInfo, blockInfo);
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
			clientMessageF("ItemID=%i extraData=%i tileName=%s", (*g_Data.getLocalPlayer()->getSelectedItem()->item)->itemId, g_Data.getLocalPlayer()->getSelectedItem()->extraData, (*g_Data.getLocalPlayer()->getSelectedItem()->item)->tileName.getText());
		}
	}
	//clientMessageF("color=%i", g_Data.getLocalPlayer()->getArmorColorInSlot(0, 0));

	if (localInfo) {
		//logF("TPS = %f", g_Data.getLocalPlayer()->getTicksPerSecond());
		logF("ScreenName = %s", g_Data.getScreenName.c_str());
		logF("yaw2 = %f,yawUnused1 = %f yawUnused2 = %f bodyYaw = %f oldBodyYaw = %f", g_Data.getLocalPlayer()->yaw2, g_Data.getLocalPlayer()->yawUnused1, g_Data.getLocalPlayer()->yawUnused2, g_Data.getLocalPlayer()->bodyYaw, g_Data.getLocalPlayer()->oldBodyYaw);
		logF("pitch = %f,pitch2 = %f", g_Data.getLocalPlayer()->pitch, g_Data.getLocalPlayer()->pitch2);
		if (g_Data.getLocalPlayer()->damageTime > 1) {
			logF("Hurttime = %i", g_Data.getLocalPlayer()->damageTime);
			logF("BPS = %f", g_Data.getLocalPlayer()->getBlocksPerSecond());
			logF("Motion = %f %f %f", g_Data.getLocalPlayer()->velocity.x, g_Data.getLocalPlayer()->velocity.y, g_Data.getLocalPlayer()->velocity.z);
		}

		/*if (abs(g_Data.getLocalPlayer()->velocity.y) > 0.f) {
			logF("velocity.y = %f", g_Data.getLocalPlayer()->velocity.y);
		}*/
	}

	if (blockInfo && GameData::isRightClickDown()) {
		Level* level = g_Data.getLocalPlayer()->level;
		auto levelBlock = g_Data.getLocalPlayer()->region->getBlock(level->block)->toLegacy();
		if (levelBlock->blockId != 0) {
			logF("BlockPos(%i,%i,%i) BlockID = %i BlockName = %c Face = %i", level->block.x, level->block.y, level->block.z, levelBlock->blockId, levelBlock->name.getText(), level->blockSide);
		}
	}
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
		logF("EntityTypeId=%i,Height=%f,Width=%f,DeathTime=%i,Hurttime=%i,CanShowName=%i,NameTag=%s", attackedEnt->getEntityTypeId(), attackedEnt->height, attackedEnt->width, attackedEnt->timeSinceDeath, attackedEnt->damageTime, attackedEnt->canShowNameTag(), attackedEnt->getNameTag()->getText());
		logF("lower(%f,%f,%f) upper(%f,%f,%f)", attackedEnt->aabb.lower.x, attackedEnt->aabb.lower.y, attackedEnt->aabb.lower.z, attackedEnt->aabb.upper.x, attackedEnt->aabb.upper.y, attackedEnt->aabb.upper.z);
		logF("Pos(%f,%f,%f)", attackedEnt->getPos()->x, attackedEnt->getPos()->y, attackedEnt->getPos()->z);
	}
}