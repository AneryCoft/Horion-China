#include "Aimbot.h"

Aimbot::Aimbot() : IModule(0, Category::COMBAT, "Automatically aims at the nearest entity.") {
	registerFloatSetting("Range", &range, range, 3.f, 8.f);
	registerBoolSetting("Require Click", &click, click);
	registerBoolSetting("Only Swords/Axes", &sword, sword);
	registerBoolSetting("Vertical", &vertical, vertical);
	registerFloatSetting("Horizontal Speed", &horizontalspeed, horizontalspeed, 30.f, 100.f);
	registerFloatSetting("Vertical Speed", &verticalspeed, verticalspeed, 30.f, 100.f);
	registerFloatSetting("Horizontal Range", &horizontalrange, horizontalrange, 20.f, 180.f);
	registerFloatSetting("Vertical Range", &verticalrange, verticalrange, 20.f, 180.f);
	registerBoolSetting("Aimlock", &lock, lock);
}

Aimbot::~Aimbot() {
}

const char* Aimbot::getModuleName() {
	return ("Aimbot");
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Aimbot::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (!localPlayer) return;

	std::vector<C_Entity*> allEntities;
	g_Data.forEachEntity([&](C_Entity* e, bool) -> void {
		allEntities.push_back(e);
	});

	vec3_t origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	//Loop through all our players and retrieve their information
	targetList.clear();
	for (size_t i = 0; i < allEntities.size(); i++) {
		C_Entity* currentEntity = allEntities.at(i);

		if (!Target::isValidTarget(currentEntity))
			continue;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < range)
			targetList.push_back(currentEntity);
	}

	if (targetList.size() > 0) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec2_t angle = origin.CalcAngle(*targetList[0]->getPos());
		vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;
		if ((appl.x < verticalrange && appl.x > -verticalrange) && (appl.y < horizontalrange && appl.y > -horizontalrange) && GameData::canUseMoveKeys()) {
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			if (sword && !(item->getItem()->isWeapon()))
				return;

			if (click && !g_Data.isLeftClickDown())
				return;

			if (!lock) {
				appl.x /= (100.f - verticalspeed);
				appl.y /= (100.f - horizontalspeed);
				if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
				if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
			}
			if (!vertical)
				appl.x = 0;
			localPlayer->applyTurnDelta(&appl);
		}
	}
}
