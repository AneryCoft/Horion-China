#include "Hitbox.h"

Hitbox::Hitbox() : IModule(0, Category::COMBAT, "Increase an entitys hitbox size.") {
	registerFloatSetting("Height", &height, height, 1.8f, 10.f);
	registerFloatSetting("Width", &width, width, 0.6f, 10.f);
}

Hitbox::~Hitbox() {
}

const char* Hitbox::getModuleName() {
	return ("Hitbox");
}

void EnableHitbox(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 10.f) {
		currentEntity->width = hitboxMod->width;
		currentEntity->height = hitboxMod->height;
	}
}

void DisableHitbox(C_Entity* currentEntity, bool isRegularEntitie) {
	if (!Target::isValidTarget(currentEntity))
		return;

	currentEntity->width = 1.8f;
	currentEntity->height = 0.6f;
} //该函数用于在关闭后恢复玩家碰撞箱 可能会把一些NPC的碰撞箱改坏

void Hitbox::onTick(C_GameMode* gm) {
	if (gm->player == nullptr)
		return;

	g_Data.forEachEntity(EnableHitbox);
}

void Hitbox::onDisable() {
	g_Data.forEachEntity(DisableHitbox);
}
