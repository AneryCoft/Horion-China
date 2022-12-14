#include "FastStop.h"

FastStop::FastStop() : IModule(0, Category::MOVEMENT, "Make a quick stop when you stop moving") {
}

FastStop::~FastStop() {
}

const char* FastStop::getModuleName() {
	return ("FastStop");
}

void FastStop::onMove(C_MoveInputHandler* input) {
	if (!g_Data.getLocalPlayer()->damageTime) {
		vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };
		if (moveVec2d.magnitude() < 0.01f) {
			g_Data.getLocalPlayer()->velocity.x = 0.f;
			g_Data.getLocalPlayer()->velocity.z = 0.f;
		}
	}
}