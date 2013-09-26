#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\gl\Texture.h>
#include <cinder\CinderMath.h>
#include <string>
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

//these all need to be virtual
class Menu {
public:
	enum state {UI_STATE_INACTIVE, UI_STATE_NORMAL, UI_STATE_HOVERING, UI_STATE_DEPRESSED};
	enum moveDir {UI_MOVE_NONE, UI_MOVE_X, UI_MOVE_Y, UI_MOVE_BOTH}; 

	Menu(Menu * parent = NULL);
	~Menu();

	void draw();
	void update();

	void addChild(Menu *c) { _childList.push_back(c); }
	bool isVecInMenu(Vec2f i) { return ((i.x >= _pos.x && i.y >= _pos.y) && (i.x <= _pos.x + _size.x && i.y <= _pos.y + _size.y)); }

	//Setters for data;
	void setPos(Vec2f i);
	void setRelativePos(Vec2f i);
	void setSize(Vec2f i) { _size = i; }
	void setColor(ColorA i) { _color = i; }
	void setColorHover(ColorA i) { _colorHover = i; }
	void setColorPress(ColorA i) { _colorPress = i; }
	void setColorInactive(ColorA i) { _colorInactive = i; }
	void setRotation(float i) { _rotation = i; }
	void setTexture(gl::Texture *i) { _tex = i; }
	void setTextrueHover(gl::Texture *i) { _texHover = i; }
	void setTexturePress(gl::Texture *i) { _texPress = i; }
	void setTextureInactive(gl::Texture *i) { _texInactive = i; }
	void setIsMovable(moveDir i);
	void setIsSelected(bool i) { _isSelected = i; }
	void setMetadata(string i) { _metaData = i; }
	
	//Getters.
	Vec2f getSliderValue();
	Vec2f getPos() const { return _pos; }
	Vec2f getRelativePos() const { return _parentOffset; }
	Vec2f getSize() const { return _size; }
	Vec2f getPosDelta() const { return _moveDelta; }
	Vec2f getClickPos() const { return _clickPos; }
	ColorA getColor() const { return _color; }
	ColorA getColorHover() const { return _colorHover; }
	ColorA getColorPress() const { return _colorPress; }
	ColorA getColorInactive() const { return _colorInactive; }
	string getMetadata() const { return _metaData; }
	float getRotation() const { return _rotation; }
	bool wasClicked();
	
	//Event pass-through
	void onMouseDown(MouseEvent);
	void onMouseUp(MouseEvent);
	void onMouseDrag(MouseEvent);
	void onMouseWheel(MouseEvent);
	void onMouseMove(MouseEvent);

	void onKeyDown(KeyEvent);
	void onKeyUp(KeyEvent);

	void lockFocus() { _focusLocked = true; }
	bool isFocusLocked() { return _focusLocked; }

protected:
	void _bindTexAndCol();
	void _unbindTex();

	//Operational mebers
	bool _wasClicked, _isClicked, _canMoveX, _canMoveY;
	bool _canSelect;
	Vec2f _clickPos, _moveDelta;
	state _state;
	string _metaData;
	string _oldMeta;

	//Data members
	Menu* _parent;
	bool _isSelected, _dragged, _focusLocked;
	Vec2f _pos, _size, _offset, _parentPos, _parentOffset;
	Vec2f _minPos, _maxPos;
	ColorA _color, _colorHover, _colorPress, _colorInactive;
	float _rotation;
	gl::Texture *_tex, *_texHover, *_texPress, *_texInactive;
	vector<Menu*> _childList;
};