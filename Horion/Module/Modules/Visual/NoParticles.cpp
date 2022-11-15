#include "NoParticles.h"

NoParticles::NoParticles() : IModule(0, Category::VISUAL, "Disables render particles.") {
}

NoParticles::~NoParticles() {
}

const char* NoParticles::getModuleName() {
	return ("NoParticles");
}