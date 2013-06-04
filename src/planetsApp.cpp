#include <cinder\app\AppNative.h>
#include <cinder\gl\gl.h>

#include "Controller.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class planetsApp: public AppNative {
  public:
	void setup();
	void mouseDown(MouseEvent event);	
	void update();
	void draw();
};

void planetsApp::setup() {
}

void planetsApp::mouseDown(MouseEvent event) {
}

void planetsApp::update() {
}

void planetsApp::draw() {
	gl::clear(Color(0, 0, 0 )); 
}

CINDER_APP_NATIVE(planetsApp, RendererGl)
