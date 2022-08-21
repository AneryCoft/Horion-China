#include "Scaffold.h"

//#include "../../../Utils/Logger.h"

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you.") {
	mode = (*new SettingEnum(this))
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("Extend", 1))
		.addEntry(EnumEntry("Staircase", 2))
		.addEntry(EnumEntry("Horizontal", 3));
	registerEnumSetting("Mode", &mode, 0);
	this->registerIntSetting("Extend Length", &this->length, this->length, 1, 10);
	registerBoolSetting("AutoBlock", &this->autoBlock, this->autoBlock);
	registerBoolSetting("blockCount", &this->showBlockCount, this->showBlockCount);
	registerBoolSetting("Rotations", &this->rotations, this->rotations);
	registerBoolSetting("Render", &this->render, this->render);
}

Scaffold::~Scaffold() {
}

const char* Scaffold::getModuleName() {
	return "Scaffold";
}

bool Scaffold::tryScaffold(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();
	renderPos = blockBelow;

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
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			//if (spoof) findBlock();
			bool idk = true;
			g_Data.getCGameMode()->buildBlock(&blok, i, idk);

			return true;
		}
	}
	return false;
}

bool Scaffold::findBlock() {
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
					return true;
				}
			}
			else {
				emptySlot = i;
			}
		}
		else {
			if (stack->item != nullptr && stack->getItem()->isBlock()) {
				inv->moveItem(i, emptySlot);
				supplies->selectedHotbarSlot = emptySlot;
				return true;
			}
		}
	}
	return false;
}

void Scaffold::calcCount() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	blockCount = 0;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != nullptr && stack->getItem()->isBlock()) {
			blockCount += stack->count;
		}
	}
}

void Scaffold::onGetPickRange() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	/*if (!g_Data.canUseMoveKeys())
		return;*/

	if (showBlockCount && blockCount == 0)
		return;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();

	if (selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()) {  // Block in hand?
		if (autoBlock) {
			findBlock();
		}
		else
			return;
	}


	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1

	switch (mode.selected) {
	case 0:
	{
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;
		rotpos = blockBelow;
		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						if (!tryScaffold(blockBelow)) {
							canrot = false;
							return;
						}
					}
				}
			}
		}
	}
	break;
	case 1:
	{
		static int  i = 0;

		float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;

		if (i > length) {
			i = 0;
		}

		if (i != 0) {
			blockBelow.x = blockBelow.x += cos(cal) * i;  // Block 1 ahead the player X
			blockBelow.z = blockBelow.z += sin(cal) * i;  // Block 1 ahead the player Z
		}
		rotpos = blockBelow;
		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;

					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						if (!tryScaffold(blockBelow)) {
							canrot = false;
							return;
						}
					}
				}
			}
		}
		++i;
	}
	break;
	case 2:
	{
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 1.5f;

		vec3_t blockBelowBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 2 blocks below the player
		blockBelowBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelowBelow.y -= 2.0f;

		rotpos = blockBelow;

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
						if (!tryScaffold(blockBelow)) {
							canrot = false;
							return;
						}
					}
				}
			}
		}
	}
	break;
	case 3:
	{
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y = horizontalHigh;
		rotpos = blockBelow;
		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						if (!tryScaffold(blockBelow)) {
							canrot = false;
							return;
						}
					}
				}
			}
		}
	}
	}
	canrot = true;
}

void Scaffold::onTick(C_GameMode* gm) {
	if (rotations) 
		g_Data.getLocalPlayer()->level->rayHitType = 0;
	
}

void Scaffold::onDisable() {
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
	//水平搭路计算脚下高度

	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
}

void Scaffold::onPlayerTick(C_Player* player) {
	if (player->isJumping())
	{
		rotpos = player->eyePos0.sub(vec3_t(0, 2, 0));
		player->pitch = 75.f;
	}
	if (rotations) {
		vec2_t joe = player->getPos()->CalcAngle(rotpos).normAngles();
		if (canrot && findBlock() && g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f) {
			player->bodyYaw = joe.y;
			player->yawUnused1 = joe.y;
			player->pitch = 75.f;
		}
	}
}

void Scaffold::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {

		if (g_Data.getLocalPlayer() != nullptr && g_Data.canUseMoveKeys() ) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			bool changepitch = true;
			if (g_Data.getLocalPlayer()->isJumping()) {
				movePacket->pitch = 75.f;
				changepitch = false;
			}
			if (rotations && canrot) {
				if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f || GameData::isKeyDown(*input->spaceBarKey)) {
					vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(rotpos);
					if (changepitch)
						movePacket->pitch = angle.x;
					movePacket->headYaw = angle.y;
					movePacket->yaw = angle.y;
				}
			}
		}
	}
}

void Scaffold::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.canUseMoveKeys()) {
		if (showBlockCount) {
			calcCount();
			vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
			std::string countText = "Blocks:" + std::to_string(blockCount);
			DrawUtils::drawText(vec2_t{ windowSize.x / 2.f, windowSize.y / 2.f + 20.f }, &countText, MC_Color(255, 255, 255), 1.3f);
			//vec4_t renderPos = {};
			//DrawUtils::fillRectangle(renderPos, MC_Color(13, 29, 48), 1.f);
		}

		if (render) {
			DrawUtils::setColor(0, 0.3705, 1, 1);
			DrawUtils::drawBox(renderPos, renderPos.add(1), (float)0.5 / (float)1.f, false);
		}
	}
}
