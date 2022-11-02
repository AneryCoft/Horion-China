#include "Scaffold.h"

//#include "../../../Utils/Logger.h"
uint8_t* Scaffold::renderItem = reinterpret_cast<uint8_t*>(FindSignature("10 48 8B 01 FF 50 28 48 8B F8 EB 07 48 8D 3D 1B 58 0C 03 48 8B 8B 60 01 00 00 48 8B 01 FF 90 C0 00 00 00 BA 01 00 00 00 48 8B 88 80 06 00 00 48 8B 01 FF 50 28 48 8B 8B 60 01"));

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("Extend", 1))
		.addEntry(EnumEntry("Staircase", 2))
		.addEntry(EnumEntry("Horizontal", 3));
	//.addEntry(EnumEntry("Hive", 4));
	registerEnumSetting("Mode", &mode, 0);
	rotations = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Extend", 1))
		.addEntry(EnumEntry("Forward", 2))
		.addEntry(EnumEntry("Back", 3));
	registerEnumSetting("Rotation", &rotations, 0);
	registerIntSetting("Extend Length", &length, length, 0, 10);
	registerBoolSetting("AutoBlocks", &autoBlock, autoBlock);
	registerBoolSetting("blockCount", &showBlockCount, showBlockCount);
	//registerBoolSetting("Rotation", &this->rotation, this->rotation);
	registerBoolSetting("Render", &render, render);
	registerBoolSetting("Lock rendered item", &renderItemBefore, renderItemBefore);

}

Scaffold::~Scaffold() {
}

const char* Scaffold::getModuleName() {
	return "Scaffold";
}

bool Scaffold::tryScaffold(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			// bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			// if (spoof) findBlock();
			blockPos = blockBelow;
			needRender = true;
			bool idk = true;
			g_Data.getCGameMode()->buildBlock(&blok, i, idk);

			return true;
		}
	}
	return false;
}

void Scaffold::selectedBlock() {
	/*__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->isBlock() && (*stack->item)->itemId != 0) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;*/
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int emptySlot = 0;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (i < 9) {
			if (stack->item != nullptr) {
				if (stack->getItem()->isBlock()) {
					supplies->selectedHotbarSlot = i;
					return;
				}
			}
			else {
				emptySlot = i;
			}
		}
		else {
			if (stack->item != nullptr && stack->getItem()->isBlock()) {
				inv->swapSlots(i, emptySlot);
				supplies->selectedHotbarSlot = emptySlot;
				return;
			}
		}
	}
}

int Scaffold::calcCount() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int blockCount = 0;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != nullptr && stack->getItem()->isBlock()) {
			blockCount += stack->count;
		}
	}
	return blockCount;
}

void Scaffold::onGetPickRange() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	else if (renderItem != nullptr && renderItemBefore && *renderItem == 0x10ui8)
		return;
	/*if (!g_Data.canUseMoveKeys())
		return;*/

	if (calcCount() == 0)
		return;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();

	if (selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()) {  // Block in hand?
		if (autoBlock) {
			selectedBlock();
		}
		else
			return;
	}

	/*if (onlyGround && !g_Data.getLocalPlayer()->onGround)
		return;*/

		// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1

	g_Data.getLocalPlayer()->level->rayHitType = 0;

	switch (mode.selected) {
	case 0: {
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
					}
				}
			}
		}
	} break;
	case 1: {
		static int i = 0;

		float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;

		if (i > length) {
			i = 0;
		}

		if (i != 0) {
			blockBelow.x = blockBelow.x += cos(cal) * i;  // Block 1 ahead the player X
			blockBelow.z = blockBelow.z += sin(cal) * i;  // Block 1 ahead the player Z
		}

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;

					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
					}
				}
			}
		}
		++i;
	} break;
	case 2: {
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 1.5f;

		vec3_t blockBelowBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 2 blocks below the player
		blockBelowBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelowBelow.y -= 2.0f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				blockBelowBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					blockBelowBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						blockBelowBelow.z += vel.z;
						blockBelowBelow.x += vel.x;
						tryScaffold(blockBelowBelow);
					}
				}
			}
		}
	} break;
		/*
	case 3: {
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y = horizontalHigh;

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
					}
				}
			}
		}
	} break;
	*/
	case 3: {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		float yaw = g_Data.getLocalPlayer()->yaw;

		if (input->rightKey) {
			yaw += 90.f;
			if (input->forwardKey)
				yaw -= 45.f;
			else if (input->backKey)
				yaw += 45.f;
		}
		if (input->leftKey) {
			yaw -= 90.f;
			if (input->forwardKey)
				yaw += 45.f;
			else if (input->backKey)
				yaw -= 45.f;
		}
		if (input->backKey && !input->leftKey && !input->rightKey)
			yaw += 180.f;

		float cal = (yaw + 90) * (PI / 180);
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y = horizontalHigh;

		blockBelow.x = blockBelow.x += cos(cal) * 0.8;  // Block 1 ahead the player X
		blockBelow.z = blockBelow.z += sin(cal) * 0.8;  // Block 1 ahead the player 

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.8f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.8f;

					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z * 0.4f;
						blockBelow.x += vel.x * 0.4f;
					}
				}
			}
		}
	}
	}
}

void Scaffold::onDisable() {
	if (renderItem != nullptr && *renderItem == 0x14ui8) {
		*renderItem = 0x10ui8;
		VirtualProtect(renderItem, sizeof(uint8_t), oldProtect, &oldProtect);
	}
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
}

void Scaffold::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	horizontalHigh = g_Data.getLocalPlayer()->eyePos0.y;
	horizontalHigh -= g_Data.getLocalPlayer()->height;
	horizontalHigh -= 0.5f;
	//Ë®Æ½´îÂ·¼ÆËã½ÅÏÂ¸ß¶È

	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;

	needRender = false;
	
	if (renderItem != nullptr && renderItemBefore) {
		VirtualProtect(renderItem, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &oldProtect);
		*renderItem = 0x14ui8;
		auto supply = g_Data.getLocalPlayer()->getSupplies();
		supply->render = supply->selectedHotbarSlot;
	}
}

void Scaffold::onPlayerTick(C_Player* player) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	else if (renderItem != nullptr && renderItemBefore && *renderItem == 0x10ui8)
		return;
	if (rotations.selected != 0 && needRender) {
		switch (rotations.selected) {
		case 1: {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockPos);
		} break;
		case 2: {
			vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
			float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
			vec3_t vel = g_Data.getLocalPlayer()->velocity;
			vel = vel.normalize();  // Only use values from 0 - 1
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.y -= 0.8f;
			blockBelow.x = blockBelow.x += cos(cal) * 1;
			blockBelow.z = blockBelow.z += sin(cal) * 1;
			blockBelow.x -= vel.x * 0.4f;
			blockBelow.z -= vel.x * 0.4f;
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
		} break;
		case 3: {
			vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
			float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
			vec3_t vel = g_Data.getLocalPlayer()->velocity;
			vel = vel.normalize();  // Only use values from 0 - 1
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.y -= 0.8f;
			blockBelow.x = blockBelow.x -= cos(cal) * 1;
			blockBelow.z = blockBelow.z -= sin(cal) * 1;
			blockBelow.x -= vel.x * 0.4f;
			blockBelow.x -= vel.z * 0.4f;
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
		}
		}
		if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f || player->isJumping()) {
			player->bodyYaw = angle.y;
			player->yawUnused1 = angle.y;
			player->pitch = angle.x;
		}
	}
}

void Scaffold::onSendPacket(C_Packet* packet, bool&) {
	if (rotations.selected != 0 && needRender) {
		if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f || g_Data.getLocalPlayer()->isJumping()) {
			/*
			switch (rotations.selected) {
			case 0: {
				angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockPos);
			} break;
			case 1: {
				vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
				float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
				vec3_t vel = g_Data.getLocalPlayer()->velocity;
				vel = vel.normalize();  // Only use values from 0 - 1
				blockBelow.y -= g_Data.getLocalPlayer()->height;
				blockBelow.y -= 0.8f;
				blockBelow.x = blockBelow.x += cos(cal) * 1;
				blockBelow.z = blockBelow.z += sin(cal) * 1;
				blockBelow.x -= vel.x * 0.4f;
				blockBelow.z -= vel.x * 0.4f;
				angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			} break;
			case 2: {
				vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
				float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
				vec3_t vel = g_Data.getLocalPlayer()->velocity;
				vel = vel.normalize();  // Only use values from 0 - 1
				blockBelow.y -= g_Data.getLocalPlayer()->height;
				blockBelow.y -= 0.8f;
				blockBelow.x = blockBelow.x -= cos(cal) * 1;
				blockBelow.z = blockBelow.z -= sin(cal) * 1;
				blockBelow.x -= vel.x * 0.4f;
				blockBelow.x -= vel.z * 0.4f;
				angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			}
			}*/
			if (packet->isInstanceOf<C_MovePlayerPacket>()) {
				C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
			/*if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
				PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
				//authInputPacket->pitch = angle.x;
				authInputPacket->pitch = 89;
				authInputPacket->yawUnused = angle.y;
				authInputPacket->yaw = angle.y;
			}*/
		}
	}
}

void Scaffold::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.canUseMoveKeys()) {
		if (showBlockCount) {
			vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
			std::string countText = "Blocks:" + std::to_string(calcCount());
			DrawUtils::drawText(vec2_t{ windowSize.x / 2.f, windowSize.y / 2.f + 20.f }, &countText, MC_Color(255, 255, 255), 1.3f);
			// vec4_t renderPos = {};
			// DrawUtils::fillRectangle(renderPos, MC_Color(13, 29, 48), 1.f);
		}

		if (render && needRender) {
			DrawUtils::setColor(0, 0.3705, 1, 1);
			DrawUtils::drawBox(blockPos, blockPos.add(1), (float)0.5 / (float)1.f, false);
		}
	}
}