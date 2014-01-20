
#include "Car.hpp"

#include <cassert>

namespace car {

Car::Car(const Vector3d& position) : position(position) {}

void Car::setMesh(irr::scene::IMeshSceneNode *newMesh) {
	mesh = newMesh;
}

void Car::move(irr::f32 deltaSeconds) {

	using namespace irr;

	const f32 cDrag = 0.5;
	const f32 cRollingResistance = 14.2;
	const f32 fEngine = 200.0;

	Vector3d velocityDirection = Vector3d(velocity).normalize();
	f32 speed = getSpeed();

	f32 engineForce = fEngine * gasLevel;
	Vector3d fTraction = velocityDirection * engineForce;
	Vector3d fDrag = -cDrag * velocity * speed;
	Vector3d fRollingResistance = -cRollingResistance * velocity;

	Vector3d fLongtitudinal = fTraction + fDrag + fRollingResistance;

	Vector3d acceleration = fLongtitudinal / mass;

	velocity += deltaSeconds * acceleration;
	position += deltaSeconds * velocity;

	updateMesh();
}

void Car::setGas(irr::f32 value) {
	assert(value >= 0.0 && value <= 1.0);
	gasLevel = value;
}

irr::f32 Car::getGas() const {
	return gasLevel;
}

void Car::increaseGas(irr::f32 deltaSeconds) {
	const irr::f32 increaseSpeed = 0.9;
	gasLevel += increaseSpeed*deltaSeconds;
	if (gasLevel > 1.) {
		gasLevel = 1.;
	}
}

void Car::decreaseGas(irr::f32 deltaSeconds) {
	const irr::f32 decreaseSpeed = 1.5;
	gasLevel -= decreaseSpeed*deltaSeconds;
	if (gasLevel < 0.) {
		gasLevel = 0.;
	}
}

const Car::Vector3d& Car::getPosition() const {
	return position;
}

const Car::Vector3d& Car::getVelocity() const {
	return velocity;
}

irr::f32 Car::getSpeed() const {
	return velocity.getLength();
}


void Car::updateMesh() {
	mesh->setPosition(position);
}

}

