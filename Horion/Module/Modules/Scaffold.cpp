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
			//bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
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
			if (mode.selected == 1 || mode.selected == 2) {
				blockPos = blockBelow;
				needRotations = true;
			}
			renderPos = blockBelow;
			needRender = true;
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
	needRotations = false;

	if (g_Data.getLocalPlayer() == nullptr)
		return;
	/*if (!g_Data.canUseMoveKeys())
		return;*/

	if (calcCount() == 0)
		return;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();

	if (selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()) {  // Block in hand?
		if (autoBlock) {
			findBlock();
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

	needRotations = true;

	switch (mode.selected) {
	case 0:
	{
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
	}
	break;
	case 1:
	{
		static int  i = 0;

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
	}
	break;
	case 3:
	{
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
	}
	}
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

	needRender = false;
	needRotations = false;
}

void Scaffold::onPlayerTick(C_Player* player) {
	if (rotations && needRotations) {
		if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f || player->isJumping()) {
			vec2_t angle = player->getPos()->CalcAngle(blockPos);
			player->bodyYaw = angle.y;
			player->yawUnused1 = angle.y;
			player->pitch = angle.x;
		}
	}
}

void Scaffold::onSendPacket(C_Packet* packet, bool& cancelSend) {
	if (rotations && needRotations) {
		if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f || g_Data.getLocalPlayer()->isJumping()) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockPos);
			if (packet->isInstanceOf<C_MovePlayerPacket>()) {
				C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
			/*if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
				PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
				authInputPacket->pitch = angle.x;
				authInputPacket->yawUnused = angle.y;
				authInputPacket->yaw = angle.y;
			}*/
		}
	}
}

void Scaffold::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.canUseMoveKeys()) {
		if (showBlockCount) {
			vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
			std::string countText = "Blocks:" + std::to_string(calcCount());
			DrawUtils::drawText(vec2_t{ windowSize.x / 2.f, windowSize.y / 2.f + 20.f }, &countText, MC_Color(255, 255, 255), 1.3f);
			//vec4_t renderPos = {};
			//DrawUtils::fillRectangle(renderPos, MC_Color(13, 29, 48), 1.f);
		}

		if (render && needRender) {
			DrawUtils::setColor(0, 0.3705, 1, 1);
			DrawUtils::drawBox(renderPos, renderPos.add(1), (float)0.5 / (float)1.f, false);
		}
	}
}

void Scaffold::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (mode.selected == 0 || mode.selected == 3) {
		blockPos = g_Data.getLocalPlayer()->eyePos0;
		blockPos.y -= g_Data.getLocalPlayer()->height;
		blockPos.y -= 0.5f;
	}
}