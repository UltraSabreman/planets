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
	void update();
	void draw();
private:
	Controller cont;
	MyCam mCam;
};

void planetsApp::setup() {
	
	CameraPersp test;
	test.setPerspective(90.0f, getWindowAspectRatio(), 3.0f, 100000.0f);
	mCam.setCam(test);

	for (int i = 0; i < 10; i++)
		cont.addRandomPlanet();
}

void planetsApp::mouseDrag(MouseEvent event) {
	mCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isRightDown());
}
void planetsApp::mouseWheel(MouseEvent event) {
	mCam.mouseWheel(event.getWheelIncrement());
}

void planetsApp::update() {
	/*Vec3f mEye = Vec3f(1000, 0, 0);
	Vec3f mCenter = Vec3f::zero();
	Vec3f mUp = Vec3f::zAxis();
	
	test.lookAt(mEye, mCenter, mUp);
	mCam.setCurrentCam(test);*/


	cont.update();
}

void planetsApp::draw() {
	gl::clear(Color(0, 0, 0)); 
	gl::setMatrices(mCam.getCam());
	gl::color(Color(255,0,0));
	gl::drawSphere(Vec3f::zero(), 10);
	gl::color(Color(0,255,0));
	gl::drawSphere(mCam.getCam().getCenterOfInterestPoint(), 10);

	cont.draw();
}

CINDER_APP_NATIVE(planetsApp, RendererGl)
