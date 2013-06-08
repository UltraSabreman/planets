#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\Camera.h>
#define E 2.71828183f
#define PI 3.14159

using namespace ci;
using namespace ci::app;
using namespace std;

class MyCam {
public:
	MyCam();
	MyCam(const CameraPersp &inputInitCam);

	void mouseWheel(float wheelSpin);
	void mouseDrag(Vec2f mousePos, bool isLeftDown, bool isRightDown);
	void getPickingRay(Vec2f mousePos, Vec3f &rayPos, Vec3f &rayDir);

	const CameraPersp& getCam() const;
	void setCam(const CameraPersp &camIn);
private:
	CameraPersp _curCam, _initCam;
	Vec2f _oldMousePos;

	void updatePickingData();

	Vec3f _viewPlane, _viewHight, _viewWidth;
	
};

