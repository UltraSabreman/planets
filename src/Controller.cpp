#include "Controller.h"


Controller::Controller() {
	_massCenter = Vec3f::zero();
}

void Controller::addRandomPlanet() {
	//Rand::randSeed(time(NULL));
	_planets.push_back(new Planet(Rand::randVec3f() * 5000, Rand::randFloat(0,500000), 1));//Rand::randFloat()));
}

Planet *Controller::pickPlanet(Vec3f startPos, Vec3f dir) {
	for (Planet *planet : _planets) 
		if (planet->isHitByRay(startPos, dir)) {
			_selected = planet;
			return planet;
		}
	_selected = NULL;
	return NULL;
}

void Controller::update(bool paused) {
	//Do the "Physics" Tick.
	//collisions
	for (Planet *planet1 : _planets) {
		Vec3f avgForce = planet1->getForce();
		for (Planet *planet2 : _planets) {
			if (planet1->isDead() || planet2->isDead()  || planet1 == planet2) 
				continue;

			if (planet1->isColliding(planet2)) {
				if (planet1->getMass() > planet2->getMass()) {
					planet1->absorb(planet2);
					planet2->setDead(true);
				}else { //do some sort of "bounce" if they are equal in mass?
					planet2->absorb(planet1);
					planet1->setDead(true);
				}
			}
		}
	} 
	remove_if(_planets.begin(), _planets.end(), [](Planet*p){ return p->isDead(); });

	if (!paused) {
		//Do Actual force calculations
		for (Planet *planet1 : _planets) {
			Vec3f avgForce = planet1->getForce();
			for (Planet *planet2 : _planets) {
				if (!planet1 || !planet2 || planet1 == planet2) 
					continue;

				//Calculate unit direction vector form particle 1, to particle 2.
				Vec3f dirVec = (planet1->getPos() - planet2->getPos()).safeNormalized();
				//Get force vector using Newtons law of universal gravitation in vector form (G_ is the gravitational constant);
				avgForce += -G * ((planet2->getMass() * planet1->getMass()) / pow(dirVec.length(), 2)) * dirVec;
			}
			planet1->setForce(avgForce);
		} 
	}


	//update the planets, and calc center of mass.
	Vec3f massPos = Vec3f::zero();
	float totalMass = 0;

	for(Planet *planet : _planets){
		planet->update(paused);
		massPos += (planet->getMass() * planet->getMass());
		totalMass += planet->getMass();
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

