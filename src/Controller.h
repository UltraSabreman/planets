#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\Rand.h>
#include <list>

#include "Planet.h"

//This is in m^3/(kg*s^2)
#define G 6.67384e-11 


using namespace ci;
using namespace ci::app;
using namespace std;

class Controller {
public:
	Controller();

	void update(bool paused = false);
	void draw();

	Planet* pickPlanet(Vec3f startPos, Vec3f dir);

	void addRandomPlanet();
private:
	list<Planet*> _planets;
	Planet *_selected;

	Vec3f _massCenter;
};