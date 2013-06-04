#pragma once
#include <cinder\app\AppNative.h>
#include <vector>

#include "Planet.h"

using namespace ci;
using namespace ci::app;

class Controller {
public:
	Controller();

	void update();
	void draw();

private:
	vector<Planet*> _planets;

	Vec2f _massCenter;

};