#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\Camera.h>
#include <vector>

#include "menu.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Ui {
public:
	Ui(float k = 0);

	//void update(Vec2i
	void update();
	void draw();

	void addElement(Menu *l) { _elements.push_back(l); }

private:
	CameraPersp _uiCam;

	vector<Menu*> _elements;

};