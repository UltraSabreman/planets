#include "uiMain.h"

Ui::Ui() { 
	_size = Vec2i(0,0);
}

Ui::Ui(Vec2i &size) {
	_size = size;
}

void Ui::update() {
	for (Menu *m : _elements)
		m->update();
}

void Ui::draw() {
	gl::enableAlphaBlending();
	
	//swap to 2d drawing and back
	//no idea how this works, ripped from stack-overflow.

	//... I should probably readup on this stuff at some point.

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, _size.x, _size.y, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);


	for (Menu *m : _elements) 
		m->draw();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();        ----and this?
	gl::disableAlphaBlending();
}
