#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0, Category::MOVEMENT, "Only need to press the key once to switch the sneak") {
	registerBoolSetting("Silent", &doSilent, doSilent); //我真不觉得这个有什么意义
}

AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

void AutoSneak::onTick(C_GameMode* gm) {
	if (!doSilent) {
		if (g_Data.canUseMoveKeys()) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

			if (GameData::isKeyDown(*input->sneakKey)) {
				if (canToggle) {
					isSneak = !isSneak;
					canToggle = false;
				}
			}
			else {
				canToggle = true;
			}

			if (isSneak) {
				g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
			}
		}
	}
}

void AutoSneak::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (doSilent) {
		C_PlayerActionPacket p;
		p.action = 12;  //停止潜行包
		p.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
	else {
		if (!g_Data.getClientInstance()->getGameSettingsInput()->sneakKey) {
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		}
	}
}

void AutoSneak::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return; //fixed crash

	if (doSilent) {
		C_PlayerActionPacket p;
		p.action = 11;  //开始潜行包
		p.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
}

void AutoSneak::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (doSilent) {
		if (packet->isInstanceOf<C_PlayerActionPacket>()) {
			auto* packets = reinterpret_cast<C_PlayerActionPacket*>(packet);
			if (packets->action == 12) {
				cancelSend = true; // Dont call LoopbackPacketSender_sendToServer
			}
		}
	}
}
