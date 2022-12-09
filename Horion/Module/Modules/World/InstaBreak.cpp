#include "InstaBreak.h"

InstaBreak::InstaBreak() : IModule(VK_NUMPAD4, Category::WORLD, "Break any block instantly.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Destroy", 0))
		.addEntry(EnumEntry("FastDig", 1))
		.addEntry(EnumEntry("The Hive", 2));
	registerEnumSetting("Mode", &mode, 0);
}

InstaBreak::~InstaBreak() {
}

const char* InstaBreak::getModuleName() {
	return ("InstaBreak");
}

void InstaBreak::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys() || !GameData::isLeftClickDown()) {
		tick = 0;
		return;
	}

	Level* level = g_Data.getLocalPlayer()->level;
	if (level->rayHitType != 0)
		return; //是否有指向方块

	++tick;

	if (tick == 6) {
		tick = 0;
	}
	else if (tick == 1) {
		C_PlayerActionPacket actionPacket;
		actionPacket.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
		actionPacket.face = level->blockSide;
		actionPacket.blockPosition = level->block;

		actionPacket.action = 0; //开始破坏
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

		actionPacket.action = 18; //继续破坏
		for (int i = 0; i < 10; i++) {
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}

		gm->destroyBlock(new vec3_ti(level->block), level->blockSide);

		actionPacket.action = 2; //停止破坏
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
	}
}