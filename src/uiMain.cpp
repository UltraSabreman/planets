#include "uiMain.h"

Ui::Ui(float k) {
	_uiCam.setPerspective(90, k, 1, 1000);
	_uiCam.lookAt(Vec3f(0,0,100), Vec3f::zero(), Vec3f::yAxis());
}

void Ui::draw() {
	gl::setMatrices(_uiCam);
	gl::enableAlphaBlending();
	
	for (Menu *m : _elements) 
		m->draw();

	gl::disableAlphaBlending();
	gl::popMatrices();
}
