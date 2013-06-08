#include "Planet.h"

Planet::Planet(Vec3f inPos, float inMass, float InDensity) {
	_pos = inPos;
	_density = InDensity;
	_mass = inMass;
	
	_radius = 0; 
	_accel = Vec3f::zero(); _force = Vec3f::zero(); 
	_vel = Vec3f::zero(); _oldPos = inPos;
	_isDead = false;
	hit = false;
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
	if (hit)
		gl::color(Color(0,255,255));
	gl::drawSphere(_pos, _radius);
}

void Planet::absorb(Planet *planet) {
	if (_isDead) return;

	//perfectly inelestic collision, might change later to be partially inelastic.
	_vel = (_mass*_vel + planet->_mass*planet->_vel) / (_mass + planet->_mass);

	_mass += planet->_mass;
}
bool Planet::isHitByRay(Vec3f startPos, Vec3f dir) {
	//this is jsut the discriminanot form the quadratic equation. If we whant to chekkc where it hits, we need to full thing.
	float sqr = pow(dir.dot(startPos - _pos), 2.0f);
	float numOfHits = sqr - dir.dot(dir) * ((startPos - _pos).dot(startPos - _pos) - pow(_radius, 2.0f));

	//use this if you need the hit pos.
	/*if (numOfHits < 0) return false;
	Vec3f firstHit = ((-dir.dot(startPos - _pos) + sqrt(numOfHits)) / dir.dot(dir)) * dir;
	Vec3f secondHit = ((-dir.dot(startPos - _pos) - sqrt(numOfHits)) / dir.dot(dir)) * dir;*/

	return (numOfHits >= 0);
}