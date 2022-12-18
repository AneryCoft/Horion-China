#include "Scaffold.h"

uint8_t* Scaffold::renderItem = reinterpret_cast<uint8_t*>(FindSignature("10 48 8B 01 FF 50 28 48 8B F8 EB 07 48 8D 3D 1B 58 0C 03 48 8B 8B 60 01 00 00 48 8B 01 FF 90 C0 00 00 00 BA 01 00 00 00 48 8B 88 80 06 00 00 48 8B 01 FF 50 28 48 8B 8B 60 01"));

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you.") {
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("Extend", 1))
		.addEntry(EnumEntry("Staircase", 2))
		.addEntry(EnumEntry("The Hive", 3));
	registerEnumSetting("Mode", &mode, 0);
	rotations = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Forward", 1))
		.addEntry(EnumEntry("Back", 2))
		.addEntry(EnumEntry("The Hive", 3));
	registerEnumSetting("Rotation", &rotations, 0);
	autoBlocks = SettingEnum(this)
		.addEntry(EnumEntry("OFF", 0))
		.addEntry(EnumEntry("Select", 1))
		.addEntry(EnumEntry("RenderSpoof", 2))
		.addEntry(EnumEntry("Spoof", 3));
	registerEnumSetting("AutoBlocks", &autoBlocks, 0);
	registerFloatSetting("Extend Length", &length, length, 0.f, 10.f);
	registerFloatSetting("Timer", &timer, timer, 20.f, 40.f);
	registerBoolSetting("Horizontal", &horizontal, horizontal);
	registerBoolSetting("BlocksCount", &showBlockCount, showBlockCount);
	registerBoolSetting("RenderBlocks", &render, render);
}

Scaffold::~Scaffold() {
}

const char* Scaffold::getModuleName() {
	return ("Scaffold");
}

bool Scaffold::tryScaffold(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();

	blockPos = blockBelow;
	needRender = true;

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer->region->getBlock(vec3_ti(blockBelow))->blockLegacy->material->isReplaceable) {
		vec3_ti block(blockBelow);

		//寻找周围可以放置的方块
		static std::array<vec3_ti, 6> checklist = {
			vec3_ti(0, -1, 0), //0
			vec3_ti(0, 1, 0), //1
			vec3_ti(0, 0, -1), //2
			vec3_ti(0, 0, 1), //3
			vec3_ti(-1, 0, 0), //4
			vec3_ti(1, 0, 0) //5
		};

		bool foundCandidate = false;
		int blockFace = 0;
		for (auto current : checklist) {
			vec3_ti calc = block.sub(current);
			if (!(localPlayer->region->getBlock(calc)->blockLegacy)->material->isReplaceable) {
				//寻找固体方块点击
				foundCandidate = true;
				block = calc;
				break;
			}
			blockFace++;
		}
		if (foundCandidate) {
			vec3_t* localPos = localPlayer->getPos();
			switch (rotations.selected) {
			case 1: { //Forward
				angle = localPos->CalcBlockAngle(blockBelow, 1);
			} break;
			case 2: { //Back
				angle = localPos->CalcBlockAngle(block.toVec3t(), blockFace);
			} break;
			}

			needRotation = true;

			if (autoBlocks.selected == 3) {
				selectedBlocks();
			}

			g_Data.getCGameMode()->buildBlock(&block, blockFace, true);

			if (autoBlocks.selected == 3) {
				localPlayer->getSupplies()->selectedHotbarSlot = prevSlot;
			}

			return true;
		}
	}
	return false;
}

void Scaffold::selectedBlocks() {
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

int Scaffold::calcBlocksCount() {
	C_Inventory* inv = g_Data.getLocalPlayer()->getSupplies()->inventory;

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
	//needRotation = false;

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;
	else if (renderItem != nullptr && *renderItem == 0x10ui8 && autoBlocks.selected == 2)
		return;
	/*if (!g_Data.canUseMoveKeys())
		return;*/

	if (calcBlocksCount() == 0)
		return;

	if (autoBlocks.selected != 3) {
		auto selectedItem = localPlayer->getSelectedItem();

		if (!selectedItem->isValid() || selectedItem->count == 0 || !selectedItem->getItem()->isBlock()) {  // Block in hand?
			if (autoBlocks.selected == 1 || autoBlocks.selected == 2) {
				selectedBlocks();
			}
			else
				return;
		}
	}

	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);

	//用velocity来调整
	float speed = localPlayer->velocity.magnitudexz(); //水平速度
	float bps = localPlayer->getBlocksPerSecond();
	vec3_t velocity = localPlayer->velocity.normalize();

	vec3_t blockBelow = *localPlayer->getPos();
	if (horizontal) {
		blockBelow.y = horizontalHigh;
	}
	else {
		blockBelow.y -= localPlayer->height;
		blockBelow.y -= 0.5f;
	}
	//玩家脚下的方块

	switch (mode.selected) {
	case 0: { //Normal
		if (!tryScaffold(blockBelow)) {
			if (speed > 0.01f) {  // Are we actually walking?
				blockBelow.z -= velocity.z * 0.5f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= velocity.x * 0.5f;
					tryScaffold(blockBelow);
					/*if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += velocity.z;
						blockBelow.x += velocity.x;
					}*/
				}
			}
		}
	} break;
	case 1: { //Extend
		float cal = (localPlayer->yaw + 90) * (PI / 180);

		for (float i = 0; i < length; i += 0.1f) {
			vec3_t tempBlockBelow = blockBelow;

			if (i != 0) {
				if (!localPlayer->isJumping() || horizontal) {
					tempBlockBelow.x += cos(cal) * i;
					tempBlockBelow.z += sin(cal) * i;
				}
			}

			if (tryScaffold(tempBlockBelow)) {
				break;
			}
		}
	} break;
	case 2: { //Staircase
		blockBelow = *localPlayer->getPos();
		blockBelow.y -= localPlayer->height;
		blockBelow.y -= 1.5f;

		vec3_t blockBelowBelow = blockBelow.sub(0.f, 1.f, 0.f);

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.01f) {  // Are we actually walking?
				blockBelow.z -= velocity.z * 0.4f;
				blockBelowBelow.z -= velocity.z * 0.4f;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= velocity.x * 0.4f;
					blockBelowBelow.x -= velocity.x * 0.4f;
					tryScaffold(blockBelow);
					tryScaffold(blockBelowBelow);
					/*if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += velocity.z;
						blockBelow.x += velocity.x;
						blockBelowBelow.z += velocity.z;
						blockBelowBelow.x += velocity.x;
						tryScaffold(blockBelowBelow);
					}*/
				}
			}
		}
	} break;
	case 3: { //The Hive
		float cal = (localPlayer->yaw + 90) * (PI / 180);

		blockBelow.x += cos(cal) * 0.8f;  // Block 1 ahead the player X
		blockBelow.z += sin(cal) * 0.8f;  // Block 1 ahead the player

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.01f) {  // Are we actually walking?
				blockBelow.z -= velocity.z * 0.8f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= velocity.x * 0.8f;
					tryScaffold(blockBelow);
				}
			}
		}
	}
	}
}

void Scaffold::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	horizontalHigh = g_Data.getLocalPlayer()->getPos()->y;
	horizontalHigh -= g_Data.getLocalPlayer()->height;
	horizontalHigh -= 0.5f;

	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;

	needRotation = false;
	needRender = false;

	if (renderItem != nullptr && autoBlocks.selected == 2) {
		VirtualProtect(renderItem, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &oldProtect);
		*renderItem = 0x14ui8;
		auto supply = g_Data.getLocalPlayer()->getSupplies();
		supply->render = supply->selectedHotbarSlot;
	}
}

void Scaffold::onDisable() {
	if (renderItem != nullptr && *renderItem == 0x14ui8) {
		*renderItem = 0x10ui8;
		VirtualProtect(renderItem, sizeof(uint8_t), oldProtect, &oldProtect);
	}

	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);

	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	}
}

void Scaffold::onPlayerTick(C_Player* player) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	else if (renderItem != nullptr && *renderItem == 0x10ui8 && autoBlocks.selected == 2)
		return;

	if (rotations.selected != 0 && needRotation) {
		if (rotations.selected == 3) { //The Hive
			vec3_t blockBelow = *g_Data.getLocalPlayer()->getPos();
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.y -= 0.8f;

			float cal = (g_Data.getLocalPlayer()->yaw + 90) * (PI / 180);
			blockBelow.x += cos(cal) * 1;
			blockBelow.z += sin(cal) * 1;

			vec3_t vel = g_Data.getLocalPlayer()->velocity.normalize();
			blockBelow.x -= vel.x * 0.4f;
			blockBelow.z -= vel.x * 0.4f;

			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
		}

		if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f) {
			player->pitch = angle.x;
			player->bodyYaw = angle.y;
			player->yawUnused1 = angle.y;
		}
	}
}

void Scaffold::onSendPacket(C_Packet* packet, bool&) {
	if (rotations.selected != 0 && needRotation) {
		if (g_Data.getLocalPlayer()->getBlocksPerSecond() > 0.1f) {
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
			std::string countText = "Blocks:" + std::to_string(calcBlocksCount());
			DrawUtils::drawText(vec2_t{ windowSize.x / 2.f, windowSize.y / 2.f + 20.f }, &countText, MC_Color(255, 255, 255), 1.3f);
			// vec4_t renderPos = {};
			// DrawUtils::fillRectangle(renderPos, MC_Color(13, 29, 48), 1.f);
		}

		if (render && needRender) {
			DrawUtils::setColor(0.f, 0.3705f, 1.f, 1.f);
			DrawUtils::drawBox(blockPos, blockPos.add(1), 0.5f, false);
		}
	}
}
