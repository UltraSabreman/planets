#pragma once
#include <cinder\app\AppNative.h>

using namespace ci;
using namespace ci::app;

class Planet {
public:
	Planet();


	void update();
	void draw();

private:
	Vec2f _pos, _accel, _vel, _force;
	float _mass, _radius, _density;
	

};