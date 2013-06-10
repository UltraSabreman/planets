#include <cinder\app\AppNative.h>
#include <cinder\gl\gl.h>

#include "Controller.h"
#include "MyCam.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class planetsApp: public AppNative {
public:
	void setup();
	void mouseWheel(MouseEvent);	
	void mouseDrag(MouseEvent);
	void mouseUp(MouseEvent);
	void keyUp(KeyEvent);
	void update();
	void draw();

private:
	Controller cont;
	MyCam mCam;

	Planet *selected;
	bool doubleClick, tracking, paused, editMode;
	float lastClickTime;

	Vec3f pos;
	Vec3f point1, point2, dir1, dir2;
};

void planetsApp::setup() {

	pos = Vec3f::zero();
	paused = false;
	tracking = false;
	doubleClick = false;
	lastClickTime = 0;
	selected = NULL;

	CameraPersp test;
	test.setPerspective(90.0f, getWindowAspectRatio(), 3.0f, 100000.0f);
	mCam.setCam(test);

	for (int i = 0; i < 30; i++)
		cont.addRandomPlanet();
}

void planetsApp::mouseDrag(MouseEvent event) {
	mCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isRightDown());
	if (event.isRightDown() && tracking)
		tracking = false;
}

void planetsApp::mouseWheel(MouseEvent event) {
	mCam.mouseWheel(event.getWheelIncrement());
}

void planetsApp::mouseUp(MouseEvent event) {
	if (event.isLeft()) {
		float curTime = getElapsedSeconds();
		if (curTime - lastClickTime <= 0.25 && !doubleClick) {
			doubleClick = true;

			Vec3f start, dir;
			mCam.getPickingRay(event.getPos(), start, dir);
			selected = cont.pickPlanet(start, dir);
			point1 = start;
			dir1 = dir * 1000;

			if (selected) 
				tracking = true;
			else if (!selected && tracking)
				tracking = false;
		}else 
			doubleClick = false;

		lastClickTime = curTime;
	}
	
	if(event.isMiddle() && tracking) {

	}
}

void planetsApp::keyUp(KeyEvent event) {
	if (event.getCode() == KeyEvent::KEY_SPACE)
		paused = !paused;

}

void planetsApp::update() {
	cont.update(paused);

	if (tracking) {
		if (!selected) {
			tracking = false;
			return;
		}
			
		

		if (!paused) {
			mCam.getCam().setCenterOfInterestPoint(selected->_pos);
			mCam.getCam().setEyePoint(mCam.getCam().getEyePoint() + selected->_vel);
		}
	}
}

void planetsApp::draw() {
	gl::clear(Color(0, 0, 0)); 

	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::setMatrices(mCam.getCam());

	gl::color(Color(255,0,0));
	gl::drawSphere(Vec3f::zero(), 5);
	gl::color(Color(0,255,0));
	gl::drawSphere(mCam.getCam().getCenterOfInterestPoint(), 5);

	cont.draw();

	if (selected) {
		gl::color(Color(255,0,0));
		gl::drawStrokedCube(selected->_pos, Vec3f(2,2,2) * selected->_radius);
		
		gl::color(Color(0,255,255));
		gl::drawLine(point1, dir1);	
		gl::drawLine(point2, dir2);	
	}

	gl::popMatrices();
	gl::disableDepthRead();
	gl::disableDepthWrite();	
}


CINDER_APP_NATIVE(planetsApp, RendererGl)
