#include "Disabler.h"

Disabler::Disabler() : IModule(0, Category::MISC, "Makes some Anti-Cheats unworkable") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Velocity Spoof", 0));
	registerEnumSetting("Mode", &mode, 0);
}

Disabler::~Disabler() {
}

const char* Disabler::getModuleName() {
	return ("Disabler");
}

void Disabler::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (mode.selected == 0) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInputPacket->velocity = vec3_t(0.f, 0.f, 0.f);
		}
	}
}
