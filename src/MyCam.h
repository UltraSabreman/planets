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
	void test(Vec2f mousePos, Vec3f &rayPos);

	Vec2f to2D(Vec3f pos);
	Vec3f to3D(Vec2f pos);

	CameraPersp& getCam();
	void setCam(const CameraPersp &camIn);
private:
	void updatePickingData();

	CameraPersp _curCam, _initCam;
	Vec2f _oldMousePos;

	Vec3f _viewDir, _viewHight, _viewWidth;
};

