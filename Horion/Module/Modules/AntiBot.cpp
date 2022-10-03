#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0, Category::PLAYER, "Enable this module to filter out bots!") {
	registerBoolSetting("Hitbox Check", &hitboxCheck, hitboxCheck);
	registerBoolSetting("Name Check", &nameCheck, nameCheck);
	registerBoolSetting("Name Check+", &nameCheckPlus, nameCheckPlus);
	registerBoolSetting("Invisible Check", &invisibleCheck, invisibleCheck);
	registerBoolSetting("EntityID Check", &entityIdCheck, entityIdCheck);
	registerBoolSetting("Move Check", &moveCheck, moveCheck);
	registerBoolSetting("GameMode Check", &modeCheck, modeCheck);
	registerBoolSetting("Advance Check", &advanceCheck, advanceCheck);
}

AntiBot::~AntiBot() {
}

const char* AntiBot::getModuleName() {
	return ("AntiBot");
}

struct advancecheck {
	int tpsCheck = 0; //速度检测
	uint8_t onGroundCheck = 0;  //0 not on ground, 1 on ground, 2 change
	float lastrange = 0.f;
	TimerUtil checkTime;
	float lastplayeryaw = 0.f;
	float lasttargetyaw = 0.f;
	float lastticktargetyaw = 0.f;
	uint8_t targetYawcheck = 0ui8; //0 未查 ,1 左 , 2 右 , 3 跳过 
	TimerUtil checkTime1;
	TimerUtil checkTime2;
	vec3_t posold;
	bool tpcheck = true;
	int tpcheckcounter = 0;
	int nottpcheckcounter = 0;

	int isbotcounter = 0;
	uint8_t isbot = 0ui8; // 0 player, 1 unknown1 , 2 unknown2 , 3 unknown3 , 4 bot
	uint64_t spawnTime = 0ui64; //
};

std::unordered_map<__int64, advancecheck> advancechecklist;

void AntiBot::onWorldTick(C_GameMode* gm) {

	if (std::time(nullptr) < g_Hooks.connecttime + 1)
		return;

	if (g_Data.getLocalPlayer() == nullptr) {
		advancechecklist.clear();
		return;
	}
	
	static TimerUtil checkTimeenable;
	 
	if (checkTimeenable.hasTimedElapsed(40, true)) {
		g_Data.forEachValidEntity([this](C_Entity* ent, bool) {
			if (![ent]() -> bool {
				__try {
					return (ent != nullptr && (__int64)ent != 0xFFFFFFFFFFFFFCD7 && (__int64)ent != 0xFFFFFF00FFFFFF00 && ent != nullptr && *(__int64*)ent != 0xFFFFFFFFFFFFFCD7 && *(__int64*)ent > 0x6FF000000000 && *(__int64*)ent < 0x800000000000 && *((int64_t*)ent + 1) < 0x6FF000000000 && *(__int64*)ent <= Utils::getBase() + 0x10000000 && ent->isPlayer());
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					return false;
				}
			}()) return;
			
			if (!ent->isPlayer())
				return;

			auto localplayer = g_Data.getLocalPlayer();

			auto aclfing = advancechecklist.find(*ent->getUniqueId());
			if (aclfing == advancechecklist.end()) {                // 没有
				auto& acl = advancechecklist[*ent->getUniqueId()];  //创建
				acl.spawnTime = time(nullptr);

				// tps
				acl.tpsCheck = 100;

				acl.isbot = 1ui8;

				if (ent->getTicksPerSecond() == 0.f) {
					if (acl.isbot = ent->getRealSpeed() == 0.7f)
						acl.isbot = 2ui8;
					//大部分情况假人都是0.7f
				} else
					acl.tpsCheck = ent->getRealSpeed() == 0.7f ? 175 : 0;  //大部分情况假人都是0.7f

				acl.onGroundCheck = ent->onGround;

				if (!ent->onGround)
					acl.tpsCheck += 125;

				acl.lastrange = ent->getPos()->sub(*localplayer->getPos()).magnitude();

				acl.lastticktargetyaw = acl.lasttargetyaw = localplayer->getPos()->CalcAngle(*ent->getPos()).y;
				acl.lastplayeryaw = localplayer->yaw;

			} else {
				auto& acl = aclfing->second;

				if (acl.isbot == 4ui8) {
					if (ent->getPos()->sub(*ent->getPosOld()).magnitude() > 0.05f) {
						//if (acl.isbotcounter++ < 3) { //误判次数
							acl.isbot = 2;
							acl.tpsCheck = 50;
						//}
					}
					return;
				}
				if (acl.isbot == 0ui8) 
					return;
				// tps
				if (acl.isbot <= 3ui8) {
					if (ent->getTicksPerSecond() == 0.f)
						acl.tpsCheck += acl.isbot;
					else
						acl.tpsCheck--;
				}

				int tpschecknum = 0;
				switch (acl.isbot) {
				case 1:
					tpschecknum = 700;
					break;
				case 2:
					tpschecknum = 550;
					break;
				case 3:
					tpschecknum = 350;
					break;
				default:
					break;
				}

				if (acl.tpsCheck >= 300 /*(hall ? 650 : 450)*/) {
					acl.isbot++;
					acl.tpsCheck = 0;
				} else if (acl.tpsCheck < 0) {
					acl.isbot--;
					acl.tpsCheck = 150;
				}

				// onFireCheck
				if (acl.isbot <= 2ui8 && (ent->isOnFire() || ent->isSneaking() || ent->fallDistance >= 1.f)) {
					acl.tpsCheck -= 5;
				}

				// onGroundCheck
				
				if (acl.isbot >= 1ui8) {
					if (ent->damageTime > 1)
						acl.checkTime2.resetTime();
					else if (acl.checkTime2.hasTimedElapsed(1000, true)) {
						if (acl.onGroundCheck != 2ui8 && !ent->canFly) {
							if (acl.onGroundCheck != ent->onGround) 
								acl.onGroundCheck = 2ui8;
							else
								acl.tpsCheck += acl.onGroundCheck == 1 ? 5 : 10;
						}
					}
				}
				
				//通常假人与你的距离保持差不多(可能会会误判外挂)
				const float dist = abs(localplayer->getPos()->sub(*ent->getPos()).magnitude());

				if (auto tps = localplayer->getPos()->sub(*localplayer->getPosOld()).magnitude(); acl.isbot > 2ui8 && tps < 0.05f) {
					if (acl.checkTime.hasTimedElapsed(1000, true)) {
						if (dist <= 5.f && dist - acl.lastrange < 0.5f)
							acl.tpsCheck += 2;
						acl.lastrange = dist;
					}
				}

				// yaw check
				//要么一直在你背后，要么围着你lastspeed转(可能会会误判外挂)

				if (acl.isbot >= 1ui8) {
					if (ent->isJumping())
						acl.isbot = 1;
					if (acl.tpcheck) {
						if (ent->getPos()->sub(*ent->getPosOld()).magnitude() == 0.f) {
							if (ent->getPos()->sub(acl.posold).magnitude() > 0.5f)
								acl.tpcheckcounter++;
							acl.posold = *ent->getPos();
							if (acl.tpcheckcounter >= 20) {
								acl.tpsCheck += acl.isbot;

								if (ent->getPos()->y > 250.f)
									acl.isbot = 4;
							}
						} else {
							acl.nottpcheckcounter++;
							if (acl.nottpcheckcounter >= 20) {
								acl.isbot--;
								acl.tpcheck = false;
							}
						}
					}
				}

				if (acl.isbot >= 3ui8) {
					auto normAngles = [](float num) noexcept -> float {
						while (num > 180.0f)
							num -= 360.0f;
						while (num < -180.0f)
							num += 360.0f;
						return num;
					};

					const auto angle = localplayer->getPos()->CalcAngle(*ent->getPos());  // x垂直,y水平
					if (dist <= 5.f && acl.checkTime1.hasTimedElapsed(250, true)) { //角度改变
						if (abs(normAngles(localplayer->yaw - acl.lastplayeryaw)) <= 60.f && abs(normAngles(angle.y - acl.lasttargetyaw)) <= 40.f)
							acl.tpsCheck += 5;

						acl.lasttargetyaw = angle.y;
						acl.lastplayeryaw = localplayer->yaw;
					}

					if (acl.targetYawcheck != 2ui8) {
						if (acl.targetYawcheck == 0ui8)
							acl.targetYawcheck = 1 + normAngles(angle.y - acl.lastticktargetyaw) < 0.f;
						else if (acl.targetYawcheck != 1 + normAngles(angle.y - acl.lastticktargetyaw) < 0.f)
							acl.targetYawcheck = 2ui8;

						acl.tpsCheck += 2;
					}
					acl.lastticktargetyaw = angle.y;

					//// tp check
				}
			}
		});
	}
}
