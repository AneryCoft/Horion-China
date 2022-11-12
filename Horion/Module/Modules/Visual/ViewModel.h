#pragma once

#include "../Module.h"

class ViewModel : public IModule {
public:
	bool Reset = false;
	bool doTranslate = true;
	bool doScale = true;
	bool doRotate = true;

	float xScale = 1.f;
	float yScale = 1.f;
	float zScale = 1.f;

	float xTrans = 0.f;
	float yTrans = 0.f;
	float zTrans = 0.f;

	float rotateAngle = 0.f;
	float xRotate = 1.f;
	float yRotate = 1.f;
	float zRotate = 1.f;

	ViewModel();
	~ViewModel();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
