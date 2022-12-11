#include "Teleport.h"

Teleport::Teleport() : IModule(0, Category::MISC, "Click a block to teleport to it.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("lerp", 1))
		.addEntry(EnumEntry("MultiPacket", 2));
		//.addEntry(EnumEntry("CubeCraft", 3));
	registerEnumSetting("Mode", &mode, 1);
	registerBoolSetting("Only Hand", &onlyHand, onlyHand);
	registerBoolSetting("Only Sneak", &onlySneak, onlySneak);
}

Teleport::~Teleport() {
}

const char* Teleport::getModuleName() {
	return "Teleport";
}

void Teleport::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys())
		return;

	if (onlyHand && g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->item != nullptr)
		return;

	if (GameData::isRightClickDown() && !hasClicked) {
		hasClicked = true;

		vec3_ti block = g_Data.getLocalPlayer()->getlevel()->block;
		if (block == vec3_ti(0, 0, 0)) return;
		pos = block.toFloatVector();
		pos.x += 0.5f;
		pos.z += 0.5f;

		tpPos = pos;
		shouldTP = true;

		if(onlySneak)
		g_Data.getGuiData()->displayClientMessageF("%sTeleport position set to %sX: %.1f Y: %.1f Z: %.1f%s. Sneak to teleport!", GREEN, GRAY, pos.x, pos.y, pos.z, GREEN);
	}
	if (!GameData::isRightClickDown())
		hasClicked = false;

	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (shouldTP && !(!GameData::isKeyDown(*input->sneakKey) && onlySneak)) {
		auto localPlayer = g_Data.getLocalPlayer();
		auto localPlayerPos = *localPlayer->getPos();
		tpPos.y += (localPlayerPos.y - gm->player->getAABB()->lower.y) + 1;  // eye height + 1
		switch (mode.selected) {
		case 0:
		{
			localPlayer->setPos(tpPos);
		}
		break;
		case 1:
		{
			float dist = localPlayerPos.dist(tpPos);
			localPlayer->lerpTo(tpPos, vec2_t(1, 1), (int)fmax((int)dist * 0.1, 1));
		}
		break;
		case 2:
		{
			int dist = (int)localPlayerPos.dist(tpPos);
			int i = (int)dist / 5;
			for (int n = 0; n < i; n++) {
				vec3_t offs = tpPos.sub(localPlayerPos).div(i).mul(n);
				C_MovePlayerPacket packet = C_MovePlayerPacket(localPlayer, localPlayerPos.add(offs));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
			}

			localPlayer->setPos(tpPos);
		}
		break;
		/*
		case 3:
		{
				C_MovePlayerPacket movePlayerPacket(localPlayer, localPlayerPos);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

				C_PlayerActionPacket actionPacket;
				actionPacket.action = 8; //ÌøÔ¾
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

				localPlayerPos.y += 0.35f;

				movePlayerPacket = C_MovePlayerPacket(localPlayer, localPlayerPos);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

				localPlayer->setPos(tpPos);
		}
		*/
		}
		if (!onlySneak)
			g_Data.getGuiData()->displayClientMessageF(" % sTeleport to % sX: % .1f Y : % .1f Z : % .1f", GREEN, GRAY, pos.x, pos.y, pos.z);

		shouldTP = false;
	}
}