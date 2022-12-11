#include "Step.h"

Step::Step() : IModule(0, Category::MOVEMENT, "Increases your step height.") {
	registerFloatSetting("Height", &height, height, 1.f, 10.f);
}

Step::~Step() {
}

const char* Step::getModuleName() {
	return ("Step");
}

void Step::onTick(C_GameMode* gm) {
	gm->player->stepHeight = height;

	/*if (gm->player->walkingIntoObj) {
		C_MovePlayerPacket movePacket;
		movePacket.Position = gm->player->getPos()->add(0.f, 1.2f, 0.f);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	}*/
}
void Step::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->stepHeight = 0.5625f;
}
