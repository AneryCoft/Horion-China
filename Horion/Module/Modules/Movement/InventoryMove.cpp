#include "InventoryMove.h"

InventoryMove::InventoryMove() : IModule(0, Category::MOVEMENT, "Move even when you have your inventory open.") {
}

InventoryMove::~InventoryMove() {
}

const char* InventoryMove::getModuleName() {
	return ("InvMove");
}
void InventoryMove ::onEnable()
{
	Utils::patchBytes((unsigned char*)Utils::getBase() + 0xF49C60, (unsigned char*)"\x48\xb8\x01\x00\x00\x00\x00\x00\x00\x00\xc3", 11);
}

void InventoryMove::onDisable()
{//ֱ���޸�������� ���ܻᵼ��ĳЩ���� ��Ϊ�˷�����ʱ����
	Utils::patchBytes((unsigned char*)Utils::getBase() + 0xF49C60, (unsigned char*)"\x48\x8b\x49\x08\x48\x8b\x01\x48\xff\xa0\xc0", 11);
}
void InventoryMove::onTick(C_GameMode* gm) {
	if (strcmp(g_Data.getScreenName.c_str(), "chat_screen") == 0)
		return;

	// onTick ������Щ�����ӳ�
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	((C_MoveInputHandler*)gm->player->getMoveInputHandler())->forward = GameData::isKeyDown(*input->forwardKey);
	((C_MoveInputHandler*)gm->player->getMoveInputHandler())->backward = GameData::isKeyDown(*input->backKey);
	((C_MoveInputHandler*)gm->player->getMoveInputHandler())->left = GameData::isKeyDown(*input->leftKey);
	((C_MoveInputHandler*)gm->player->getMoveInputHandler())->right = GameData::isKeyDown(*input->rightKey);
	((C_MoveInputHandler*)gm->player->getMoveInputHandler())->isSneakDown = GameData::isKeyDown(*input->sneakKey);
	*(bool*)(((__int64)((__int64)(((__int64)((__int64)&((C_MoveInputHandler*)gm->player->getMoveInputHandler())->forward))) - 4))) = GameData::isKeyDown(*input->spaceBarKey);
	//if (gm->player->onGround && GameData::isKeyDown(*input->spaceBarKey))
	//	gm->player->jumpFromGround();
}
