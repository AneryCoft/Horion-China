#pragma once
#include "ICommand.h"
#include "../../Module/ModuleManager.h"
#include "../../path/goals/PathGoalXYZ.h"
#include "../../path/goals/PathGoalXZ.h"
#include "../../path/goals/PathGoalY.h"

class PathCommand : public IMCCommand {
public:
	PathCommand();
	~PathCommand();

	virtual bool execute(std::vector<std::string>* args);
};