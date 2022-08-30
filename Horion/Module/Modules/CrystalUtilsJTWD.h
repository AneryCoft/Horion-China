#pragma once
#pragma once
#include "../ModuleManager.h"
#include "Module.h"

struct CrystalPlacements {
	vec3_t toPlace;
	float enemyDmg = 0.f;
	float selfDmg = 0.f;
};

struct CrystalInfo {
	CrystalPlacements CPlacements;
	C_Entity* ent;
};

struct lineResults {
	int blockCount = 0;
	vec3_t lastSolidBlock;
};

inline lineResults countBlksAlongLine(vec3_t start, vec3_t end) {
	vec3_t endf = end.floor();
	vec3_t startf = start.floor();

	lineResults rtn;

	if (endf == startf) {
		rtn.blockCount = 0;
		rtn.lastSolidBlock = startf;
		return rtn;
	}

	float dist = startf.dist(endf);

	float dX = endf.x - startf.x;
	float dY = endf.y - startf.y;
	float dZ = endf.z - startf.z;

	int steps = 200;
	float segment = dist / steps;
	vec3_t seggs3d = vec3_t((dX / steps), (dY / steps), (dZ / steps));

	vec3_t imHere = startf;
	vec3_ti inCoord = startf;

	while (steps-- >= 0) {
		if ((vec3_ti)imHere.floor() != inCoord) {
			inCoord = imHere.floor();

			if (!g_Data.getLocalPlayer()->region->getBlock(inCoord)->blockLegacy->material->isReplaceable) {  // if inCoord is a block
				rtn.blockCount = rtn.blockCount + 1;
				rtn.lastSolidBlock = inCoord.toVec3t();
			}
		}

		imHere = imHere.add(seggs3d);
	}
	return rtn;
}

class CrystalPlace : public IModule {
private:
	int origSlot;
	int ctr = 0;

	vec3_t toPlace;

	bool currentlySwitchingHotbar = false;

public:
	std::vector<CrystalInfo> CJTWDPlaceArr;
	vec2_t rotAngle;
	bool placeArrEmpty = false;
	bool rotUpNow = false;

	int delay = 0;  // Time to wait (in ticks) until to place a new crystal
	//int placetimes = 1;     // Number of times the client should keep retrying to place a crystal
	int maxProximity = 5;   // What is the maximum distance can a crystal be placed from a person before switching axis
	int range = 10;         // Range for the enemies to be added to target list
	float placeRange = 10;  // Range to place endCrystals
	SettingEnum priority;   // Decides how targets are sorted (distance-> lowest to highest
							//								   health-> lowest to highest)
	int thruWallsR = 5;     // How many blocks you are allowed to place through walls
	float postWallsR = 3;   // Maximum distance to place *after* going through a wall

	/* SOON
	int lookAhead = 5;		// how many ticks in the future to predict future enemy location (set to 0 to disable predict)
	*/

	SettingEnum calcDmgType;  // What method is used to calculate damage (java, bedrock)

	bool safetyFirst = true;
	bool ReturnOnEat = true;// Whether to prioritize (reduce) self-damage over (maximizing) enemy damage
	float minHealth = 5.f;    // What is the minimum health you should have to stop placing crystals
	float maxSelfDmg = 5.f;   // Maximum damage allowable to player
	float minEnemDmg = 10.f;  // How much damage the crystals do to enemies, ON MINIMUM

	SettingEnum facePlaceType;  // Controls faceplacing (none->obvious
								//                       smart->faceplaces only when the enemy is at a certain health)
	float fpThresh = 10.f;      // Only when facePlaceType is set to smart [if this is set to 20, client always faceplaces]
	float dmgAtThresh = 2.f;    // When fpThreshold is reached, how much damage do the crystals need to deliver

	bool renderPlacing = true;  // Whether to render place position
	bool noCheckUpper = false;  // Put true if crystals can fit under blocks(endzone 1.13+ Java)
	bool attackMulti = false;   // Whether player should try to attack more than 1 enemy
	SettingEnum switchType;     // Type of Switch (regular->switch to crystal at start
								//                 switchBack->something like citySwitch in surround
								//                 APVPSpoof->Uses mob equipment as a spoofer[only works on APVP)

	SettingEnum rotate;  // Rotations (none->obvious
						 //			  normal->clientside rotate
						 //			  silent->sends packet to go look at the crystal;
						 //           pitchUp->so CA works on APVP

	/* TODO
	*  when APVPSpoof on, stop crystalling on mine
	*  fix crashing when there is no crystal and apvpspoof is on
	*  steal silent rots from ng
	*/

	/* TODO (in CrystalBreak):
	* anti-weakness
	* anti blocking crystals
	*/

	CrystalPlace();
	~CrystalPlace();

	bool isPlaceValid(vec3_t location, C_Entity* atkObj);

	// T H E  H O L Y  T R I N I T Y
	float computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg);
	float getBlastDamageEnchantReduction(C_ItemStack* armor);
	//float calculateBlockDensity(C_Entity* targetEntity, vec3_t explosionPosition);

	std::vector<CrystalPlacements> generateValidPlacements(C_Entity* target, int yOffset);

	CrystalPlacements bestPlaceOnPlane(C_Entity* target, int yOffset);

	CrystalPlacements CrystalAuraJTWD(C_Entity* target);

	bool getCrystalSpoof();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onWorldTick(C_GameMode* gm) override;
	//virtual void onPlayerTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet);
};

class CrystalBreak : public IModule {
private:
	int oldSlot;

	int Mdel = 0;  // main delay
	int Rdel = 0;  // retry delay

public:
	vec2_t rotAngleB;
	bool breakArrEmpty = false;

	int aSlot;

	int breakdelay = 0;  // Time to wait (in ticks) until to break a new crystal

	int breakRange = 10;  // Range to break endCrystals

	bool breakAll = true;  // Whether to break ALL the crystals or just the nearest one

	int breakWalls = 5;    // How many blocks you are allowed to place through walls
	float postBWalls = 3;  // Maximum distance to place *after* going through a wall

	float breakHealth = 5.f;  // What is the minimum health you should have to stop breaking crystals
	float BmaxSelfDmg = 5.f;  // Maximum damage allowable to player

	bool antiWeakness = false;  // detects if you have been hit by weakness and switches to a weapon accordingly

	bool renderBreaking = true;  // Whether to render crystal break attempts

	SettingEnum rotateBreak;  // Rotations (none->obvious
							  //			  normal->clientside rotate
							  //			  silent->sends packet to go look at the crystal;

	bool configHelper = true;

	void sortCrystals();
	void findAFuckingWeapon();

	CrystalBreak();
	~CrystalBreak();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet);
};