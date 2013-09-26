#include <cinder\app\AppNative.h>
#include <cinder\gl\gl.h>
#include <cinder\Camera.h>
#include <cinder\ImageIo.h>

#include "Controller.h"
#include "MyCam.h"
#include "uiMain.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class planetsApp: public AppNative {
public:
	void setup();
	void mouseWheel(MouseEvent);	
	void mouseDrag(MouseEvent);
	void mouseDown(MouseEvent);
	void mouseUp(MouseEvent);
	void mouseMove(MouseEvent);
	void keyUp(KeyEvent);
	void update();
	void draw();

private:
	Controller cont;
	MyCam mCam;

	Ui *test;

	int number, pause;

	Planet *selected;
	bool doubleClick, tracking, paused, editMode;
	float lastClickTime;
};

void planetsApp::setup() {

	paused = false;
	tracking = false;
	doubleClick = false;
	lastClickTime = 0;
	selected = NULL;

	CameraPersp h;
	h.setPerspective(75.0f, getWindowAspectRatio(), 3.0f, 100000.0f);
	mCam.setCam(h);

	for (int i = 0; i < 30; i++)
		cont.addRandomPlanet();

	test = new Ui(getWindowSize());
	Menu *lol = new Menu();
	lol->setPos(Vec2f(0, 0));
	lol->setSize(Vec2f(100,100));
	lol->setColor(ColorA(1,0,0,1));
	lol->setColorHover(ColorA(0,1,0,1));
	lol->setColorPress(ColorA(0,0,1,1));
	/*gl::Texture *tex = new gl::Texture(loadImage(loadAsset("but.png")));
	gl::Texture *tex2 = new gl::Texture(loadImage(loadAsset("but2.png")));
	lol->setTexture(tex);
	lol->setTexturePress(tex2);*/
	
	Menu *rofl = new Menu(lol);
	rofl->setRelativePos(Vec2f(20, 20));
	rofl->setSize(Vec2f(30,30));
	rofl->setColor(ColorA(0,0,0,1));
	rofl->setColorHover(ColorA(0.1,0.1,0.1,1));
	rofl->setColorPress(ColorA(0.2,0.2,0.2,1));
	rofl->setIsMovable(Menu::UI_MOVE_NONE);
	rofl->setMetadata("Lolol");
	
	
	pause = test->addElement(lol);
	number = test->addElement(rofl);
}

void planetsApp::mouseDrag(MouseEvent event) {
	if (test->onMouseDrag(event)) return;

	mCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isRightDown());
	if (event.isRightDown() && tracking)
		tracking = false;
}

void planetsApp::mouseDown(MouseEvent event) {
	test->onMouseDown(event);
}

void planetsApp::mouseMove(MouseEvent event) {
	test->onMouseMove(event);
}

void planetsApp::mouseWheel(MouseEvent event) {
	mCam.mouseWheel(event.getWheelIncrement());
}

void planetsApp::mouseUp(MouseEvent event) {
	

	if (event.isLeft()) {
		if (test->onMouseUp(event) ) return;

		double curTime = getElapsedSeconds();
		if (curTime - lastClickTime <= 0.25 && !doubleClick) {
			doubleClick = true;

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
	
	if(event.isMiddle() && tracking) {

	}
}

void planetsApp::keyUp(KeyEvent event) {
	if (event.getCode() == KeyEvent::KEY_SPACE)
		paused = !paused;

}

void planetsApp::update() {

	if (test->getElement(number)->wasClicked()) {
		cont.addRandomPlanet();
	}
	if (test->getElement(pause)->wasClicked()) {
		paused = !paused;
	}

	cont.update(paused);
	test->update();

	if (tracking) {
		if (!selected) {
			tracking = false;
			return;
		}
			
		
		if (!paused) {
			mCam.getCam().setCenterOfInterestPoint(selected->getPos());
			mCam.getCam().setEyePoint(mCam.getCam().getEyePoint() + selected->getVel());
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
		gl::drawStrokedCube(selected->getPos(), Vec3f(2,2,2) * selected->getRadius());
	}
	
	gl::disableDepthRead();
	gl::disableDepthWrite();

	test->draw();

	gl::popMatrices();	
}


CINDER_APP_NATIVE(planetsApp, RendererGl)
