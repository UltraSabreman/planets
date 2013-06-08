#include <cinder\app\AppNative.h>
#include <cinder\gl\gl.h>
#include <cinder\MayaCamUI.h>

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

	void doPicking();
private:
	Controller cont;
	MyCam mCam;

	Planet *selected;
	bool doubleClick, tracking, paused;
	float lastClickTime;

	Vec3f pos;
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
			Vec3f start, dir;
			mCam.getPickingRay(event.getPos(), start, dir);
			selected = cont.pickPlanet(start, dir);

			if (selected) 
				tracking = true;
			else if (!selected && tracking)
				tracking = false;
		}else 
			doubleClick = false;

		lastClickTime = curTime;
	}
	
	if(event.isRight() && selected) {
		mCam.test(event.getPos(), pos);
		//pos = selected->_pos + pos * 100;
		pos = selected->_pos + pos;

		//pos *= selected->_radius;
		//pos = pos - mCam.getCam().getEyePoint() + selected->_pos;
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
			
		mCam.getCam().setCenterOfInterestPoint(selected->_pos);

		if (!paused)
			mCam.getCam().setEyePoint(mCam.getCam().getEyePoint() + selected->_vel);
	}
}

void planetsApp::draw() {
	gl::clear(Color(0, 0, 0)); 

	gl::enableDepthRead();

	gl::setMatrices(mCam.getCam());
	gl::color(Color(255,0,0));
	gl::drawSphere(Vec3f::zero(), 10);
	gl::color(Color(0,255,0));
	gl::drawSphere(mCam.getCam().getCenterOfInterestPoint(), 10);

	cont.draw();

	if (selected) {
		gl::color(Color(255,0,0));
		gl::drawStrokedCube(selected->_pos, Vec3f(2,2,2) * selected->_radius);
	}

	gl::disableDepthRead();

	if (selected) {
		gl::color(Color(0,255,255));
		gl::drawLine(selected->_pos, pos);
		gl::color(Color(255,0,255));
		gl::drawSphere(Vec3f(mCam.to2D(selected->_oldPos),0), 5);
		
	}
	
	


}

void planetsApp::doPicking() {

}

CINDER_APP_NATIVE(planetsApp, RendererGl)
