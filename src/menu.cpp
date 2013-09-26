#include "menu.h"

Menu::Menu(Menu * parent) {
	_parent = parent;
	if (_parent) { 
		_parent->addChild(this); //builds menu tree.
		_minPos = _parent->getPos();
		_maxPos = _parent->getPos() + _parent->getSize();
	} else {
		_minPos = Vec2f::zero();
		_maxPos = getWindowSize();
	}


	_size = Vec2i::zero(); _pos = Vec2i::zero(); _parentPos = Vec2f::zero(); _parentOffset = Vec2f::zero();
	_color = ColorA(1,1,1,1); _colorHover = ColorA(1,1,1,1); _colorPress = ColorA(1,1,1,1);
	_tex = NULL; _texHover = NULL; _texPress = NULL; _texInactive = NULL;
	_rotation = 0;

	_state = UI_STATE_NORMAL;
	_metaData = "";
	_oldMeta = "";
	_wasClicked = false; _canMoveX = true; _canMoveY = true;
	_canSelect = true; _isClicked = false; _isSelected = false;
	_dragged = false; _focusLocked = false;
}

void Menu::update() {
	if (_parent) { 
		_minPos = _parent->getPos();
		_maxPos = _parent->getPos() + _parent->getSize() - _size;

		if (_parent->getPos() != _parentPos)
			setPos(_parent->getPos() + _parentOffset);

		_parentPos = _parent->getPos();
	} else {
		_minPos = Vec2f::zero();
		_maxPos = getWindowSize() - _size;
	}

	if (!_canSelect)
			_state = UI_STATE_INACTIVE;
	else if (_isSelected) { 
		if (_isClicked)
			_state = UI_STATE_DEPRESSED;
		else
			_state = UI_STATE_HOVERING;
	} else {
		_state = UI_STATE_NORMAL;
	}
}

void Menu::draw() {
	_bindTexAndCol();
	gl::drawBillboard(Vec3f(_pos + (_size/2), 0), _size, _rotation, Vec3f::xAxis(), Vec3f::yAxis());
	_unbindTex();

	if (_metaData != "") {
		gl::drawStringCentered(_metaData, _pos + (_size/2), Color(1,1,1));
	}
}

void Menu::onMouseUp(MouseEvent event) {
	if (_dragged) {
		_dragged = false;
		return;
	}

	_wasClicked = true;
	_isClicked = false;

	if (!_canSelect) return;
}

void Menu::onMouseDown(MouseEvent event) {
	_isClicked = true;
	_clickPos = event.getPos();

	if (!_canSelect) return;
	
	_offset = _pos - event.getPos();	
}

void Menu::onMouseDrag(MouseEvent event) {
	_dragged = true;
	if (event.isLeftDown() && _canSelect) {
		if (!_canMoveX && !_canMoveY) return;

		Vec2f newPosition = _offset + event.getPos(); //moves it relative to cursor.
		
		if (_parent) {
			newPosition -= _parent->getPos();
			Vec2f newPos = _pos - _parent->getPos();

			if (_canMoveX) 
				newPos.x = newPosition.x;
			if (_canMoveY)
				newPos.y = newPosition.y;
				
			setRelativePos(newPos);
		} else {
			Vec2f newPos = _pos;
			if (_canMoveX) 
				newPos.x = newPosition.x;
			if (_canMoveY)
				newPos.y = newPosition.y;

			setPos(newPos);
		}
	}
}

void Menu::onMouseMove(MouseEvent event) {

}

void Menu::_bindTexAndCol() {
	if (_state == UI_STATE_HOVERING) {
		gl::color(_colorHover);
		
		if (_texHover)
			_texHover->enableAndBind();
		else if (_tex)
			_tex->enableAndBind();
	} else if (_state == UI_STATE_DEPRESSED) {
		gl::color(_colorPress);

		if (_texPress)
			_texPress->enableAndBind();
		else if (_tex)
			_tex->enableAndBind();
	} else if (_state == UI_STATE_INACTIVE) {
		gl::color(_colorInactive);

		if (_texInactive)
			_texInactive->enableAndBind();
		else if (_tex)
			_tex->enableAndBind();
	} else {
		gl::color(_color);

		if (_tex)
			_tex->enableAndBind();
	}
}

void Menu::_unbindTex() {
	if (_state == UI_STATE_HOVERING) {
		if (_texHover)
			_texHover->unbind();
		else if (_tex)
			_tex->unbind();
	} else if (_state == UI_STATE_DEPRESSED) {
		if (_texPress)
			_texPress->unbind();
		else if (_tex)
			_tex->unbind();
	} else if (_state == UI_STATE_INACTIVE) {
		if (_texInactive)
			_texInactive->unbind();
		else if (_tex)
			_tex->unbind();
	} else {
		if (_tex)
			_tex->unbind();
	}
}

void Menu::setRelativePos(Vec2f i) { 
	if (_parent) {
		_parentOffset = i; 
		Vec2f newPos = _parent->getPos() + _parentOffset;

		setPos(newPos);
	} else 
		setPos(i);
}

bool Menu::wasClicked() { 
	if (_wasClicked) {
		_wasClicked = false;
		return true;
	} else
		return false;
}

void Menu::setIsMovable(moveDir i) {
	if (i == UI_MOVE_NONE) {
		_canMoveX = false;
		_canMoveY = false;
	} else if (i == UI_MOVE_X) {
		_canMoveX = true;
		_canMoveY = false;
	} else if (i == UI_MOVE_Y) {
		_canMoveX = false;
		_canMoveY = true;
	} else if (i == UI_MOVE_BOTH) {
		_canMoveX = true;
		_canMoveY = true;
	}
}


void Menu::setPos(Vec2f i) {
	Vec2f newPos = _pos;

	newPos.x = math<float>::clamp(i.x, _minPos.x, _maxPos.x);
	newPos.y = math<float>::clamp(i.y, _minPos.y, _maxPos.y);

	_moveDelta = newPos - _pos;
	_pos = newPos;
}

Vec2f Menu::getSliderValue() {
	if (!_parent) return Vec2f::zero();

	return (_pos - _parent->getPos());
}