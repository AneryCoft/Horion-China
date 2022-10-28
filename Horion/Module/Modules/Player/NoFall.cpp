#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode = SettingEnum(this)
		//.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Mineplex", 0))
		.addEntry(EnumEntry("The Hive", 1))
		.addEntry(EnumEntry("CubeCraft", 2))
		.addEntry(EnumEntry("Nukkit", 3))
		.addEntry(EnumEntry("WaterBucket", 4));

	registerEnumSetting("Mode", &mode, 0);
}

NoFall::~NoFall() {
}

const char* NoFall::getModuleName() {
	return ("NoFall");
}

void NoFall::onSendPacket(C_Packet* packet, bool& cancelSend) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->fallDistance > 3.f) {
		if (mode.selected == 0) {
			if (packet->isInstanceOf<C_MovePlayerPacket>()) {
				C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				movePacket->onGround = true;
			}
		}
	}
}

float NoFall::distanceToGround() {
	vec3_t localPlayerPos = *g_Data.getLocalPlayer()->getPos();

	for (int i = floor(localPlayerPos.y - 1.f); i > -65; i--) {
		vec3_ti blockPos = vec3_ti(floor(localPlayerPos.x), i, floor(localPlayerPos.z));
		int blockId = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;
		if (blockId != 0)
			return localPlayerPos.y - (float)blockPos.y - 1.f - 1.62f;
	}
}

bool NoFall::selectWaterBucket() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	prevSlot = supplies->selectedHotbarSlot;
	/*
	C_ItemStack* stack = inv->getItemStack(prevSlot);
	clientMessageF("%i", (*stack->item)->itemId); //��ʱ��ȡˮͰ��ID
	*/

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr && (*stack->item)->itemId == 362) {
			if (prevSlot != n)
				supplies->selectedHotbarSlot = n;
			return true;
		}
	}
	return false;
} //�л�ˮͰ

void NoFall::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t* localPlayerPos = gm->player->getPos();

	switch (mode.selected) {
		/*
case 0:{
	C_PlayerActionPacket actionPacket;
	actionPacket.action = 7; //Respawn
	actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
}
	break;
	*/

	case 1: {
		if (localPlayer->fallDistance > 3.5f) {
			localPlayerPos->y += 20;
			gm->player->tryTeleportTo(*localPlayerPos, true, true, 1, 1);
			gm->player->onGround = true;
			gm->player->fallDistance = 0.f;
		}
	} break;
	case 2: {
		if (localPlayer->fallDistance > 2.5f) {
			localPlayer->velocity.y = 0.f;
			localPlayer->setPos(localPlayerPos->add(0.1f));
		}
	}break;
	case 3: {
		if (localPlayer->fallDistance > 3.f) {
			C_PlayerActionPacket actionPacket;
			actionPacket.action = 15;  //�����ʳ�
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}
	}break;
	case 4:
	{
		static bool canBuild = true; //��ֹ��η���
		static int delay = 0; //ʹ����Ʒ���л���Ʒ��֮����ӳ٣�������ӳٽ��л����ˣ�

		if (localPlayerPos->y < -64.f)
			return; //������������

		if (localPlayer->fallDistance > 2.5f && canBuild) {
			for (int i = (int)localPlayerPos->y - 1; i > -65; i--) {
				blockPos = vec3_ti(floor(localPlayerPos->x), i, floor(localPlayerPos->z));
				int blockID = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (blockID != 0 && blockID != 8 && blockID != 9) //����������ˮ��ˮ
					break;

				if (i == -64 && blockID == 0)
					return; //����Ϊ���
			} //�������ʱ���µĵķ���

			//if (localPlayerPos->dist(blockPos.toVec3t()) < 5.f) {
			if (localPlayerPos->y - (float)blockPos.y < 7.12f) {
				if (selectWaterBucket()) {
					++delay;
					if (delay == 2) {
						gm->buildBlock(&blockPos, 1, true);
						canBuild = false;
						delay = 0;
					}
				}
			}
		}

		if (localPlayer->fallDistance < 1.f && !canBuild) {
			++delay;
			if (delay == 2) {
				gm->buildBlock(&blockPos, 1, true);
			} //�ջ�ˮ

			if (delay == 3) {
				localPlayer->getSupplies()->selectedHotbarSlot = prevSlot; //�л�ԭ������Ʒ
				canBuild = true;
				delay = 0;
			}
		}
	}
	}
}
