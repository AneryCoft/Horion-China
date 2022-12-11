#include "AntiAFK.h"

AntiAFK::AntiAFK() : IModule(0, Category::PLAYER, "Prevent you from getting kicked out when you are away from keyboard") {
}

AntiAFK::~AntiAFK() {
}

const char* AntiAFK::getModuleName() {
	return ("AntiAFK");
}

void AntiAFK::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (packet->isInstanceOf<C_InteractPacket>()) {
		C_InteractPacket* interactPacket = reinterpret_cast<C_InteractPacket*>(packet);
		if (interactPacket->action == 4 /*Êó±êĞüÍ£*/ ) {
			cancelSend = true;
		}
	}
}