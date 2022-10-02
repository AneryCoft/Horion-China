#include "Disabler.h"

Disabler::Disabler() : IModule(0, Category::MISC, "Makes some Anti-Cheats unworkable") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Velocity Spoof", 0))
		.addEntry(EnumEntry("0 CPS", 1));
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
	else if (mode.selected == 1) {
		if (packet->isInstanceOf<LevelSoundEventPacket>()) {
			LevelSoundEventPacket* soundEventPacket = reinterpret_cast<LevelSoundEventPacket*>(packet);
			if (soundEventPacket->sound == 43 || soundEventPacket->sound == 42) //sound 42�ǿջ���ʱ����ֵ Ҳ�ᱻ�����CPS ���ǹ�����ʱ�򲻷��Ǹ��� 
				//soundEventPacket->sound = 0; 
				cancelSend = true;
		} //�ƹ�EaseCation������CPS��� 
	}
}
