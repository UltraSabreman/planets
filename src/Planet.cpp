#include "Planet.h"

Planet::Planet(Vec3f inPos, float inMass, float InDensity) {
	_pos = inPos;
	_density = InDensity;
	_mass = inMass;
	
	_radius = 0; 
	_accel = Vec3f::zero(); _force = Vec3f::zero(); 
	_vel = Vec3f::zero(); _oldPos = inPos;
	_isDead = false;
}

bool Planet::isColliding(Planet* other) {
	if (_isDead) return false;

	return (_pos.distance(other->_pos) <= other->_radius + _radius);
}

void Planet::update() {
	if (_isDead) return;
	_radius = (pow(_mass, 1.0/3.0)/_density) * (3.0/4.0) * PI;

	//verlet integration for updating the position.
	_accel = (_force / _mass);
	_vel = (_pos - _oldPos);

	_oldPos = _pos;
	_pos = _pos + _vel + _accel;
}

void Planet::draw() {
	if (_isDead) return;

	gl::color(Color(255,255,255));
	gl::drawSphere(_pos, _radius);
}

void Planet::absorb(Planet *planet) {
	if (_isDead) return;

	//perfectly inelestic collision, might change later to be partially inelastic.
	_vel = (_mass*_vel + planet->_mass*planet->_vel) / (_mass + planet->_mass);

	_mass += planet->_mass;
}