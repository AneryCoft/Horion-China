#include "TestModule.h"
#include "../../../../Utils/Logger.h"
#include "../../../../Utils/VoxelIntersector.h"
#include "../../../DrawUtils.h"
#include "../../../../SDK/MatrixStack.h"
#include "../../../../Utils/Json.hpp"
#include "../../../SDK/Tag.h"

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
	registerBoolSetting("KillAura", &killAura, killAura);

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
	if (localInfo) {
		logF("ScreenName = %s", g_Data.getScreenName.c_str());
	}

	if (g_Data.getLocalPlayer() == nullptr) {
		return;
	}

	if (itemInfo) {
		auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();

		if (selectedItem->isValid()) {
			logF("ItemID=%i extraData=%i name=%s tileName=%s", (
				*selectedItem->item)->itemId
				, selectedItem->extraData
				, (*selectedItem->item)->name.getText()
				, (*selectedItem->item)->tileName.getText());

			if (selectedItem->tag != nullptr) {
				std::stringstream build;
				selectedItem->tag->write(build);
				auto builtStr = build.str();
				logF("NBT=%s", builtStr.c_str());
			}
			/*
			if ((*selectedItem->item)->isArmor()) {
				logF("ArmorKnockbackResistance=%f", (*selectedItem->item)->getArmorKnockbackResistance()); //锟铰斤拷辖锟斤拷锟阶撅拷为0.225f
			}
			*/
		}
	}

	if (localInfo) {
		auto localPlayer = g_Data.getLocalPlayer();
		//logF("ArmorColor = %i", g_Data.getLocalPlayer()->getArmorColorInSlot(0,0));
		logF("ScreenName=%s", g_Data.getScreenName.c_str());
		float yawSpeedInDegreesPerTick = localPlayer->getYawSpeedInDegreesPerSecond() / *g_Data.getClientInstance()->minecraft->timer;
		logF("YawSpeed=%f", yawSpeedInDegreesPerTick);
		logF("RayHitType=%i", localPlayer->level->rayHitType); //0方块 2? 1实体 3无
		logF("Motion(X=%f Y=%f Z=%f)", localPlayer->velocity.x, localPlayer->velocity.y, localPlayer->velocity.z);
		logF("yaw2 = %f,yawUnused1 = %f yawUnused2 = %f bodyYaw = %f oldBodyYaw = %f", g_Data.getLocalPlayer()->yaw2, g_Data.getLocalPlayer()->yawUnused1, g_Data.getLocalPlayer()->yawUnused2, g_Data.getLocalPlayer()->bodyYaw, g_Data.getLocalPlayer()->oldBodyYaw);
	}

	if (blockInfo) {
		Level* level = g_Data.getLocalPlayer()->level;
		auto levelBlock = g_Data.getLocalPlayer()->region->getBlock(level->block)->toLegacy();
		if (!level->rayHitType) {
			logF("BlockPos(X=%i,Y=%i,Z=%i) BlockID=%i BlockName=%s Face=%i"
				, level->block.x, level->block.y, level->block.z,
				levelBlock->blockId
				, levelBlock->name.getText()
				//, levelBlock->tileName.getText()
				, level->blockSide);
		}
	}

	if (killAura) {
		g_Data.forEachEntity([&](C_Entity* entity, bool valid) {
			if ((*entity->getPos()).dist(*g_Data.getLocalPlayer()->getPos()) <= 3.6f) {
				if (entity != g_Data.getLocalPlayer()) {
					g_Data.getCGameMode()->attack(entity);
				}
			}
			}); //简单的杀戮光环 攻击所有实体
	}
}

void TestModule::onMove(C_MoveInputHandler* hand) {
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!g_Data.canUseMoveKeys())
		return;

	Level* level = g_Data.getLocalPlayer()->level;
	if (!level->rayHitType) {
		C_BlockLegacy* levelBlock = g_Data.getLocalPlayer()->region->getBlock(level->block)->toLegacy();
		std::string blockNameAndID = std::string(levelBlock->name.getText() + std::string(" ID:") + std::to_string(levelBlock->blockId));
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		DrawUtils::drawText(vec2_t(windowSize.x / 2 - DrawUtils::getTextWidth(&blockNameAndID) / 2.f, windowSize.y / 2 + 10.f), &blockNameAndID, MC_Color(255, 255, 255), 1.f, 1.f);
	}
}

void TestModule::onSendPacket(C_Packet* packet, bool& cancelSend) {
}

void TestModule::onDisable() {
}

void TestModule::onLevelRender() {
}

void TestModule::onAttack(C_Entity* attackedEnt) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	}

	if (entityInfo) {
		logF("EntityTypeId=%i Height=%f Width=%f DeathTime=%i Hurttime=%i CanShowName=%i"
			, attackedEnt->getEntityTypeId()
			, attackedEnt->height
			, attackedEnt->width
			, attackedEnt->timeSinceDeath
			, attackedEnt->damageTime
			, attackedEnt->canShowNameTag());
		logF("NameTag=%s", attackedEnt->getNameTag()->getText());
		logF("lower(%f,%f,%f) upper(%f,%f,%f)", attackedEnt->aabb.lower.x, attackedEnt->aabb.lower.y, attackedEnt->aabb.lower.z, attackedEnt->aabb.upper.x, attackedEnt->aabb.upper.y, attackedEnt->aabb.upper.z);
		logF("Pos(X=%f,Y=%f,Z=%f)", attackedEnt->getPos()->x, attackedEnt->getPos()->y, attackedEnt->getPos()->z);
	}
}
