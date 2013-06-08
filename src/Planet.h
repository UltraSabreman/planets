#pragma once
#include <cinder\app\AppNative.h>
#define PI 3.14159

using namespace ci;
using namespace ci::app;

class Planet {
public:
	Planet(Vec3f inPos = Vec3f::zero(), float inMass = 0, float InDensity = 0);

	void update();
	void draw();

	bool isColliding(Planet *tarPlanet);
	void absorb(Planet *tarPlanet);
	bool isHitByRay(Vec3f startPos, Vec3f dir);

	//this is probably very bad, but honestly, I couldnt care less right now
	//If I actualy take this project anywhere, I'll probably swap it out to a private struct
	//and add some modirator function or something.
	Vec3f _pos, _oldPos, _accel, _vel, _force;
	float _mass, _radius, _density;
	bool _isDead, hit;
private:

};