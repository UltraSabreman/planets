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

	int addElement(Menu *l) { _elements.push_back(l); return _elements.size() - 1; }
	Menu *getElement(int i) { return _elements.at(i); }

	void onMouseDown(MouseEvent e) { if (_selected) _selected->onMouseDown(e); }
	void onMouseUp(MouseEvent e) { if (_selected) _selected->onMouseUp(e);}
	void onMouseDrag(MouseEvent e) { if (_selected) _selected->onMouseDrag(e);}
	void onMouseWheel(MouseEvent e) { if (_selected) _selected->onMouseWheel(e); }

	void onMouseMove(MouseEvent e) { getSelected(e.getPos()); if (_selected) _selected->onMouseMove(e); }

	void onKeyDown(KeyEvent e);
	void onKeyUp(KeyEvent e);

	bool focus();
	bool takeInput() { return _grabInput; }

private:
	Menu* getSelected(Vec2f i);

	Vec2i _size, _mousePos;

	bool _grabInput;

	string _newMeta, _oldMeta;

	vector<Menu*> _elements;
	Menu *_selected;

};