#include "PacketLogger.h"
#include "../../../../Utils/Logger.h"

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

	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		if (showAuth) {
			auto packets = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s Pos(X=%f Y=%f Z=%f) pitch=%f yaw=%f yawUnused=%f velocity(x=%f y=%f z=%f) inputMode=%i", packetName, packets->pos.x, packets->pos.y, packets->pos.z, packets->pitch, packets->yaw, packets->yawUnused, packets->velocity.x, packets->velocity.y, packets->velocity.z, packets->inputMode);
		}
		return;
	}
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto packets = reinterpret_cast<C_MovePlayerPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s Pos(X=%f Y=%f Z=%f) onGround=%i pitch=%f yaw=%f headYaw=%f mode=%i", packetName, packets->Position.x, packets->Position.y, packets->Position.z, (int)packets->onGround, packets->pitch, packets->yaw, packets->headYaw, packets->mode);
		return;
	}
	if (packet->isInstanceOf<C_PlayerActionPacket>()) {
		auto packets = reinterpret_cast<C_PlayerActionPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s entityRuntimeId=%i action=%i face=%i blockPos(X=%i Y=%i Z=%i)", packetName, packets->entityRuntimeId, packets->action, packets->face, packets->blockPosition.x, packets->blockPosition.y, packets->blockPosition.z);
		return;
	}
	if (packet->isInstanceOf<LevelSoundEventPacket>()) {
		auto packets = reinterpret_cast<LevelSoundEventPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s sound=%i extraData=%i disableRelativeVolume=%i isBabyMod=%i Pos(X=%f,Y=%f,Z=%f)", packetName, packets->sound, packets->extraData, (int)packets->disableRelativeVolume, (int)packets->isBabyMod, packets->pos.x, packets->pos.y, packets->pos.z);
		return;
	}
	if (packet->isInstanceOf<C_AnimatePacket>()) {
		auto packets = reinterpret_cast<C_AnimatePacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i rowingTime=%f entityId=%i", packetName, (int)packets->action, packets->rowingTime, packets->entityRuntimeId);
		return;
	}
	if (packet->isInstanceOf<NetworkLatencyPacket>()) {
		auto packets = reinterpret_cast<NetworkLatencyPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s timeStamp=%i sendBack=%i", packetName, (int)packets->timeStamp, (int)packets->sendBack);
		return;
	}
	if (packet->isInstanceOf<C_InteractPacket>()) {
		auto packets = reinterpret_cast<C_InteractPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i mousePos(X=%f,Y=%f,Z=%f)", packetName, packets->action, packets->mousePosition.x, packets->mousePosition.y, packets->mousePosition.z);
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
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s itemId=%i hotbarSlot=%i inventorySlot=%i", packetName, itemId, (int)packets->hotbarSlot1, (int)packets->inventorySlot1);
		return;
	}
	if (packet->isInstanceOf<CommandRequestPacket>()) {
		auto packets = reinterpret_cast<CommandRequestPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s command=%s type=%i", packetName, packets->command.getText(), packets->type);
		return;
	}
	if (packet->isInstanceOf<ActorEventPacket>()) {
		auto packets = reinterpret_cast<ActorEventPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s eventId=%i itemId=%i", packetName, packets->eventId, packets->itemId);
		return;
	}
	if (packet->isInstanceOf<C_TextPacket>()) {
		auto packets = reinterpret_cast<C_TextPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s xboxUserId=%s messageType=%i", packetName, packets->xboxUserId.getText(), packets->messageType);
		return;
	}

	//if (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") != 0) {
	g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s", packetName);
	//}
#endif  // DEBUG

}
