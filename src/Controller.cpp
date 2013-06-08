#include "Controller.h"

Controller::Controller() {
	_massCenter = Vec3f::zero();
}

void Controller::addRandomPlanet() {
	_planets.push_back(new Planet(Rand::randVec3f() * 5000, Rand::randFloat(0,500000), 1));//Rand::randFloat()));
}

void Controller::update() {
	//Do the "Physics" Tick.

	//collisions
	for (Planet *planet1 : _planets) {
		Vec3f avgForce = planet1->_force;
		for (Planet *planet2 : _planets) {
			if (planet1->_isDead || planet2->_isDead  || planet1 == planet2) 
				continue;

			if (planet1->isColliding(planet2)) {
				if (planet1->_mass > planet2->_mass) {
					planet1->absorb(planet2);
					planet2->_isDead = true;
				}else { //do some sort of "bounce" if they are equal in mass?
					planet2->absorb(planet1);
					planet1->_isDead = false;
				}
			}
		}
	} 
	remove_if(_planets.begin(), _planets.end(), [](Planet*p){ return p->_isDead; });

	//Do Actual force calcs
	for (Planet *planet1 : _planets) {
		Vec3f avgForce = planet1->_force;
		for (Planet *planet2 : _planets) {
			if (!planet1 || !planet2 || planet1 == planet2) 
				continue;

			//Calculate unit direction vector form particle 1, to particle 2.
			Vec3f dirVec = (planet1->_pos - planet2->_pos).safeNormalized();
			//Get force vector using Newtons law of universal gravitation in vector form (G_ is the gravitational constant);
			avgForce += -G * ((planet2->_mass * planet1->_mass) / pow(dirVec.length(), 2)) * dirVec;
		}
		 //set the force, make it visible.
		//avgForce *= FORCE_MUL;
		planet1->_force = avgForce;
	} 


	//update the planets, and calc center of mass.
	Vec3f massPos = Vec3f::zero();
	float totalMass = 0;

	for(Planet *planet : _planets){
		planet->update();
		massPos += (planet->_mass * planet->_pos);
		totalMass += planet->_mass;
	}

	if (totalMass > 0)
		_massCenter = (massPos / totalMass);
}

void Controller::draw() {
	for(Planet *planet : _planets){
		planet->draw();
		gl::color(Color(0,0,255));
		gl::drawCube(_massCenter, Vec3f(100,100,100));
	}
}

