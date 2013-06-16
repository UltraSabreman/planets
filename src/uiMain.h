#pragma once
#include <cinder\app\AppNative.h>
#include <vector>

#include "menu.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Ui {
public:
	Ui();
	Ui(Vec2i &size);

	//void update(Vec2i
	void update();
	void draw();

	void addElement(Menu *l) { _elements.push_back(l); }

	void onMouseDown(MouseEvent e) { for (Menu* m:_elements) m->onMouseDown(e); }
	void onMouseUp(MouseEvent e) { for (Menu* m:_elements) m->onMouseUp(e); }
	void onMouseDrag(MouseEvent e) { for (Menu* m:_elements) m->onMouseDrag(e); }
	void onMouseWheel(MouseEvent e) { for (Menu* m:_elements) m->onMouseWheel(e); }
	void onMouseMove(MouseEvent e) { for (Menu* m:_elements) m->onMouseMove(e); }

	void onKeyDown(KeyEvent e) { for (Menu* m:_elements) m->onKeyDown(e); }
	void onKeyUp(KeyEvent e) { for (Menu* m:_elements) m->onKeyUp(e); }

private:
	Vec2i _size;

	vector<Menu*> _elements;

};