#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\Camera.h>
#define E 2.71828183f

using namespace ci;
using namespace ci::app;
using namespace std;

class MyCam {
public:
	MyCam();
	MyCam(const CameraPersp &inputInitCam);

	void mouseWheel(float wheelSpin);
	void mouseDrag(Vec2f mousePos, bool isLeftDown, bool isRightDown);

	const CameraPersp& getCam() const;
	void setCam(const CameraPersp &camIn);
private:
	CameraPersp _curCam, _initCam;
	Vec2f _oldMousePos;
	
};
