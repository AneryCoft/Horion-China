#include "PacketLogger.h"

PacketLogger::PacketLogger() : IModule(0, Category::MISC, "Logging Packets!") {
	registerBoolSetting("ShowAuth", &showAuth, showAuth);
}

PacketLogger::~PacketLogger() {
}

const char* PacketLogger::getModuleName() {
	return ("PacketLogger");
}

void PacketLogger::onSendPacket(C_Packet* packet, bool& cancelSend) {
#ifdef _DEBUG
	char* packetName = packet->getName()->getText();

	if (showAuth) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			auto packets = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s Pos(X=%f Y=%f Z=%f) pitch=%f yaw=%f yawUnused=%f velocity(x=%f y=%f z=%f)", packetName, packets->pos.x, packets->pos.y, packets->pos.z, packets->pitch, packets->yaw, packets->yawUnused, packets->velocity.x, packets->velocity.y, packets->velocity.z);
			return;
		}
	}
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto packets = reinterpret_cast<C_MovePlayerPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s Pos(X=%f Y=%f Z=%f) onGround=%i pitch=%f yaw=%f headYaw=%f", packetName, packets->Position.x, packets->Position.y, packets->Position.z, (int)packets->onGround, packets->pitch, packets->yaw, packets->headYaw);
		return;
	}
	if (packet->isInstanceOf<C_PlayerActionPacket>()) {
		auto packets = reinterpret_cast<C_PlayerActionPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i face=%i blockPos(X=%i Y=%i Z=%i)", packetName, packets->action, packets->face,packets->blockPosition.x, packets->blockPosition.y, packets->blockPosition.z);
		return;
	}
	if (packet->isInstanceOf<LevelSoundEventPacket>()) {
		auto packets = reinterpret_cast<LevelSoundEventPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s sound=%i extraData=%i disableRelativeVolume=%i isBabyMod=%i Pos(X=%f,Y=%f,Z=%f)", packetName, packets->sound, packets->extraData, (int)packets->disableRelativeVolume, (int)packets->isBabyMod,packets->pos.x, packets->pos.y, packets->pos.z);
		return;
	}
	if (packet->isInstanceOf<C_AnimatePacket>()) {
		auto packets = reinterpret_cast<C_AnimatePacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i rowingTime=%f", packetName, (int)packets->action, packets->rowingTime);
		return;
	}
	if (packet->isInstanceOf<NetworkLatencyPacket>()) {
		auto packets = reinterpret_cast<NetworkLatencyPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s timeStamp=%f sendBack=%i", packetName, packets->timeStamp, (int)packets->sendBack);
		return;
	}
	if (packet->isInstanceOf<C_InteractPacket>()) {
		auto packets = reinterpret_cast<C_InteractPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i", packetName, packets->action);
		return;
	}
	if (packet->isInstanceOf<C_MobEquipmentPacket>()) {
		auto packets = reinterpret_cast<C_MobEquipmentPacket*>(packet);
		int itemId;
		if ((packets->item).item != nullptr) {
			itemId = (packets->item).getItem()->itemId;
		}
		else {
			itemId = 0;
		}
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s itemId=%i hotbarSlot=%i inventorySlot=%i", packetName, itemId,(int)packets->hotbarSlot1, (int)packets->inventorySlot1);
		return;
	}

	if (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") != 0) {
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s", packetName);
	}
#endif  // DEBUG

}
