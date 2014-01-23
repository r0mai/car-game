
#include "Car.hpp"

#include <cassert>

namespace car {

Car::Car(const Vector3d& position) : position(position) {}

void Car::setMesh(irr::scene::ISceneNode *newMesh) {
	mesh = newMesh;
}

void Car::move(irr::f32 deltaSeconds) {

	using namespace irr;

	const f32 cDrag = 0.5;
	const f32 cRollingResistance = 14.2;
	const f32 fEngine = 2000.0;
	const f32 fBrake = 3000.0;
	const f32 gravity = 9.8; //m/s^2

	//CM == Center of Mass
	//CG == Center of Gravity
	const f32 rearCMDistance = 1.0;
	const f32 frontCMDistance = 1.5;
	const f32 heightOfCG = 1.5;
	const f32 wheelBase = rearCMDistance + frontCMDistance;

	Vector3d velocityDirection = Vector3d(velocity).normalize();
	if (velocityDirection == Vector3d(0, 0, 0)) {
		velocityDirection = orientation;
	}
	f32 speed = getSpeed();
	f32 weight = mass * gravity;

	f32 engineForce = fEngine * gasLevel;
	f32 brakeForce = fBrake * brakeLevel;

	Vector3d fTraction = velocityDirection * engineForce;
	Vector3d fBraking = -velocityDirection * brakeForce;
	Vector3d fDrag = -cDrag * velocity * speed;
	Vector3d fRollingResistance = -cRollingResistance * velocity;

	Vector3d fLongtitudinal = fTraction + fBraking + fDrag + fRollingResistance;

	acceleration = fLongtitudinal / mass;

	f32 weightFront = (rearCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(acceleration.getLength());
	f32 weightRear = (frontCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(acceleration.getLength());

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

void Car::setBrake(irr::f32 value) {
	assert(value >= 0.0 && value <= 1.0);
	brakeLevel = value;
}

irr::f32 Car::getBrake() const {
	return brakeLevel;
}

void Car::increaseBrake(irr::f32 deltaSeconds) {
	const irr::f32 increaseSpeed = 0.9;
	brakeLevel += increaseSpeed*deltaSeconds;
	if (brakeLevel > 1.) {
		brakeLevel = 1.;
	}
}

void Car::decreaseBrake(irr::f32 deltaSeconds) {
	const irr::f32 decreaseSpeed = 1.5;
	brakeLevel -= decreaseSpeed*deltaSeconds;
	if (brakeLevel < 0.) {
		brakeLevel = 0.;
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

const Car::Vector3d& Car::getAcceleration() const {
	return acceleration;
}

void Car::updateMesh() {
	mesh->setPosition(position);
}

}

