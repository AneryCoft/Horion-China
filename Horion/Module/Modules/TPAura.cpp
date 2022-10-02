#include "TPAura.h"

TPAura::TPAura() : IModule(0x0, Category::COMBAT, "TP like fuck so you dont get hit") {
	position = SettingEnum(this)
		.addEntry(EnumEntry("Around", 0))
		.addEntry(EnumEntry("Above", 1))
		.addEntry(EnumEntry("Behind", 2));
	registerEnumSetting("Position", &position, 0);
	this->registerFloatSetting("Range", &range, range, 3.f, 20.f);
	this->registerIntSetting("TP delay", &delay, delay, 0, 20);
	this->registerBoolSetting("Lerp", &lerp, lerp);
}

TPAura::~TPAura() {
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

static std::vector<C_Entity*> targetList;

void findTarget(C_Entity* currentEntity, bool isRegularEntity) {
	static auto TPAuraMod = moduleMgr->getModule<TPAura>();

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < TPAuraMod->range) {
		targetList.push_back(currentEntity);
	}
}


void TPAura::onTick(C_GameMode* gm) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();

	if (!player->isAlive())
		return;

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachValidEntity(findTarget);

	Odelay++;

	if (!targetList.empty() && Odelay >= delay) {
		vec3_t pos = *targetList[0]->getPos();
		switch (position.selected) {
		case 0:
		{
			pos.y += 1.f;
			if (turn == 0) {
				pos.x += -1.f;
				pos.z += 1.f;
			}
			else if (turn == 1) {
				pos.x += -1.f;
				pos.z += -1.f;
			}
			else if (turn == 2) {
				pos.x += 1.f;
				pos.z += -1.f;
			}
			else if (turn == 3) {
				pos.x += 1.f;
				pos.z += 1.f;
				turn = 0;
			}
			++turn;
		} //»·ÈÆ
		break;
		case 1:
		{
			pos.y += 2.f;
		} //Í·¶¥
		break;
		case 2:
		{
			float yaw = targetList[0]->yaw - 180.f;
			pos.x -= sin(yaw * (PI / 180)) * 2.5f;
			pos.y += 1.f;
			pos.z += cos(yaw * (PI / 180)) * 2.5f;
		} //±³ºó
		}

		if (lerp) {
			float dist2 = gm->player->getPos()->dist(pos);
			player->lerpTo(pos, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
			player->lerpMotion(pos);
		}
		else {
			player->setPos(pos);
		}
		Odelay = 0;
	}
}

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
		return;
	}
}