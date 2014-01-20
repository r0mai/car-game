
#include "Car.hpp"

namespace car {

Car::Car(const Vector3d& position) : position(position) {}

void Car::setMesh(irr::scene::IMeshSceneNode *newMesh) {
	mesh = newMesh;
}

void Car::move(irr::f32 deltaSeconds) {

	using namespace irr;

	const f32 cDrag = 0.5;
	const f32 cRollingResistance = 14.2;

	Vector3d velocityDirection = Vector3d(velocity).normalize();
	f32 speed = getSpeed();

	f32 engineForce = 200.0;
	Vector3d fTraction = velocityDirection * engineForce;
	Vector3d fDrag = -cDrag * velocity * speed;
	Vector3d fRollingResistance = -cRollingResistance * velocity;

	Vector3d fLongtitudinal = fTraction + fDrag + fRollingResistance;

	Vector3d acceleration = fLongtitudinal / mass;

	velocity += deltaSeconds * acceleration;
	position += deltaSeconds * velocity;

	updateMesh();
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

