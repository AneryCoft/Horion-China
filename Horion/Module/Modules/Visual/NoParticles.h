#pragma once

#include "../Module.h"

class NoParticles : public IModule {
public:

	NoParticles();
	~NoParticles();

	const char* getModuleName() override;
};
