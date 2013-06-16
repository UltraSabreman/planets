#pragma once
#include <cinder\Vector.h>
#include <cinder\Color.h>
#include <cinder\gl\gl.h>
#define PI 3.14159

using namespace ci;

class Planet {
public:
	Planet(Vec3f inPos = Vec3f::zero(), float inMass = 0, float InDensity = 0);

	void update(bool paused = false);
	void draw();

	bool isColliding(Planet *tarPlanet);
	void absorb(Planet *tarPlanet);
	bool isHitByRay(Vec3f startPos, Vec3f dir);


	Vec3f getPos() const { return _pos; }
	Vec3f getVel() const { return _vel; }
	Vec3f getAccel() const { return _accel; }
	Vec3f getForce() const { return _force; }

	float getMass() const { return _mass; }
	float getRadius() const { return _radius; }
	float getDensity() const { return _density; }

	bool isDead() const { return _isDead; }

	void setPos(Vec3f &i) { _pos = i; }
	void setVel(Vec3f &i) { _vel = i; }
	void setAccel(Vec3f &i) { _accel = i; }
	void setForce(Vec3f &i) { _force = i; }
	void setMass(float i) { _mass = i; }
	void setRadius(float i) { _radius = i; }
	void setDensity(float i) { _density = i; }
	void setDead(bool i) { _isDead = i; }

private:
	Vec3f _pos, _oldPos, _accel, _vel, _force;
	float _mass, _radius, _density;
	bool _isDead;
};