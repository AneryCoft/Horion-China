#pragma once
#include "../Module.h"
#include "../../../Utils/Target.h"
#include <set>
#include <filesystem>

class KillInsult : public IModule {
private:
	bool mention = true;

	std::set<C_Entity*> attackList = {};
	std::vector<C_Entity*> eraseList = {};
	void getInsultMessages();
	std::vector<std::string> InsultMessageVector;
public:

	KillInsult();
	~KillInsult();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onAttack(C_Entity* attackedEnt) override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};