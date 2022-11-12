#include "ViewModel.h"

ViewModel::ViewModel() : IModule(0, Category::VISUAL, "Custom item view model.") {
	registerBoolSetting("Reset", &Reset, Reset);
	registerBoolSetting("Translate", &doTranslate, doTranslate);
	registerBoolSetting("Scale", &doScale, doScale);
	registerBoolSetting("Rotate", &doRotate, doRotate);

	registerFloatSetting("TranslateX", &xTrans, xTrans, -2.f, 2.f);
	registerFloatSetting("TranslateY", &yTrans, yTrans, -2.f, 2.f);
	registerFloatSetting("TranslateZ", &zTrans, zTrans, -2.f, 2.f);

	registerFloatSetting("ScaleX", &xScale, xScale, 0.f, 2.f);
	registerFloatSetting("ScaleY", &yScale, yScale, 0.f, 2.f);
	registerFloatSetting("ScaleZ", &zScale, zScale, 0.f, 2.f);

	registerFloatSetting("Rotate Angle", &rotateAngle, rotateAngle, 0.f, 6.3f);
	registerFloatSetting("RotateX", &xRotate, xRotate, 0.f, 2.f);
	registerFloatSetting("RotateY", &yRotate, yRotate, 0.f, 2.f);
	registerFloatSetting("RotateZ", &zRotate, zRotate, 0.f, 2.f);
}

ViewModel::~ViewModel() {
}

const char* ViewModel::getModuleName() {
	return ("ViewModel");
}

void ViewModel::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (Reset) {
		xTrans = 0.f;
		yTrans = 0.f;
		zTrans = 0.f;

		xScale = 1.f;
		yScale = 1.f;
		zScale = 1.f;

		xRotate = 1.f;
		yRotate = 1.f;
		zRotate = 1.f;

		Reset = false;
	}
}

