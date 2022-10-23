#include "NoPacket.h"

NoPacket::NoPacket() : IModule(VK_NUMPAD7, Category::MISC, "Prevents you from sending InventoryTransaction packets.") {
}

NoPacket::~NoPacket() {
}

const char* NoPacket::getModuleName() {
	return ("NoPacket");
}
bool NoPacket::allowAutoStart() {
	return false;
}
void NoPacket::onEnable() {
	if(!g_Data.isInGame())
		setEnabled(false);
}

void NoPacket::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (g_Data.isInGame())
		cancelSend = true;
}
