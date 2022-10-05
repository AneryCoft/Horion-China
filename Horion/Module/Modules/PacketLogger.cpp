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
	if (showAuth) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			auto packets = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s X=%f Y=%f Z=%f onGround=%i pitch=%f yaw=%f yawUnused=%f velocity.x=%f velocity.y=%f velocity.z=%f", packet->getName()->getText(), packets->pos.x, packets->pos.y, packets->pos.z, packets->pitch, packets->yaw, packets->yawUnused, packets->velocity.x, packets->velocity.y, packets->velocity.z);
			return;
		}
	}
	
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto packets = reinterpret_cast<C_MovePlayerPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s X=%f Y=%f Z=%f onGround=%i pitch=%f yaw=%f headYaw=%f", packet->getName()->getText(), packets->Position.x, packets->Position.y, packets->Position.z, (int)packets->onGround, packets->pitch, packets->yaw, packets->headYaw);
		return;
	}
	if (packet->isInstanceOf<C_PlayerActionPacket>()) {
		auto packets = reinterpret_cast<C_PlayerActionPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i face=%i", packet->getName()->getText(), packets->action, packets->face);
		return;
	}
	if (packet->isInstanceOf<LevelSoundEventPacket>()) {
		auto packets = reinterpret_cast<LevelSoundEventPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s sound=%i extraData=%i disableRelativeVolume=%i isBabyMod=%i", packet->getName()->getText(), packets->sound, packets->extraData, (int)packets->disableRelativeVolume, (int)packets->isBabyMod);
		return;
	}
	if (packet->isInstanceOf<C_AnimatePacket>()) {
		auto packets = reinterpret_cast<C_AnimatePacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i rowingTime=%f", packet->getName()->getText(), (int)packets->action, packets->rowingTime);
		return;
	}
	if (packet->isInstanceOf<NetworkLatencyPacket>()) {
		auto packets = reinterpret_cast<NetworkLatencyPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s timeStamp=%i sendBack=%i", packet->getName()->getText(), (int)packets->timeStamp, (int)packets->sendBack);
		return;
	}

	if (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") != 0) {
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
	}
#endif  // DEBUG

}
