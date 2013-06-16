#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\gl\Texture.h>

using namespace ci;
using namespace ci::app;

//these all need to be virtual
class Menu {
public:
	Menu(Menu * parent = NULL);

	void draw();
	void update();

	void setPos(Vec2f &i) { _pos = i; }
	void setSize(Vec2f &i) { _size = i; }
	void setColor(ColorA &i) { _color = i; }
	void setRotation(float i) { _rotation = i; }
	void setTexture(gl::Texture *i) { _texture = i; }
	
	Vec2f getPos() const { return _pos; }
	Vec2f getSize() const { return _size; }
	ColorA getColor() const { return _color; }
	float getRotation() const { return _rotation; }
	

	void onMouseDown(MouseEvent);
	void onMouseUp(MouseEvent);
	void onMouseDrag(MouseEvent);
	void onMouseWheel(MouseEvent);
	void onMouseMove(MouseEvent);

	void onKeyDown(KeyEvent);
	void onKeyUp(KeyEvent);

protected:
	Menu* _parent;

	Vec2f _pos, _size;
	ColorA _color;
	float _rotation;

	gl::Texture *_texture;

};