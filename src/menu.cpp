#include "menu.h"

Menu::Menu(Menu * parent) {
	_parent = parent;
	_size = Vec2i::zero();
	_pos = Vec2i::zero();
	_color = ColorA(1,1,1,1);
	_texture = NULL;
	_rotation = 0;
}

void Menu::draw() {
	gl::color(_color);
	if (_texture)
		_texture->bind();
	gl::drawBillboard(Vec3f(_pos, 0), _size, _rotation, Vec3f::xAxis(), Vec3f::yAxis());
	if (_texture)
		_texture->unbind();
}