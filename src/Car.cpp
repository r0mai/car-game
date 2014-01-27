
#include "Car.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "util.hpp"

namespace car {

Car::Car(const sf::Vector2f& position) : position(position) {}

void Car::move(float deltaSeconds) {


	const float cDrag = 0.5;
	const float cRollingResistance = 14.2;
	const float fEngine = 2000.0;
	const float fBrake = 3000.0;
	const float gravity = 9.8; //m/s^2
	const float transMissionEfficiency = 0.7;
	const float gearRatio = 2.1;
	const float differentialRatio = 3.42;
	const float wheelRadius = 0.34; //m
	const float mass = 1500; //kg
	const float maxTurnAngle = 0.52; //radians ~= 30 degrees

	//CM == Center of Mass
	//CG == Center of Gravity
	const float rearCMDistance = 1.0;
	const float frontCMDistance = 1.5;
	const float heightOfCG = 1.5;
	const float wheelBase = rearCMDistance + frontCMDistance;

	sf::Vector2f velocityDirection = [&] {
		return orientation;
	}();
	velocity = orientation*getLength(velocity); //hack, TODO something

	float speed = getSpeed();
	float weight = mass * gravity;

	float engineForce = fEngine * throttleLevel;
	float brakeForce = fBrake * brakeLevel;

	sf::Vector2f fTraction = velocityDirection * engineForce;
	sf::Vector2f fBraking = -velocityDirection * brakeForce;
	sf::Vector2f fDrag = -cDrag * velocity * speed;
	sf::Vector2f fRollingResistance = -cRollingResistance * velocity;

	sf::Vector2f fLongtitudinal = fTraction + fBraking + fDrag + fRollingResistance;

	acceleration = fLongtitudinal / mass;

	float weightFront = (rearCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(getLength(acceleration));
	float weightRear = (frontCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(getLength(acceleration));

	velocity += deltaSeconds * acceleration;
	position += deltaSeconds * velocity;

	if (std::abs(turnLevel) > 0.0001) {
		float steeringAngle = maxTurnAngle * turnLevel;
		float turnRadius = wheelBase / std::sin(steeringAngle);

		float angularVelocity = getLength(velocity) / turnRadius;
		sf::Transform rotateTransform;
		rotateTransform.rotate(angularVelocity*deltaSeconds * 180.f/M_PI);
		orientation = rotateTransform.transformPoint(orientation);
	}
}

void Car::setThrottle(float value) {
	assert(value >= 0.0 && value <= 1.0);
	throttleLevel = value;
}

float Car::getThrottle() const {
	return throttleLevel;
}

void Car::increaseThrottle(float deltaSeconds) {
	const float increaseSpeed = 0.9;
	throttleLevel += increaseSpeed*deltaSeconds;
	if (throttleLevel > 1.) {
		throttleLevel = 1.;
	}
}

void Car::decreaseThrottle(float deltaSeconds) {
	const float decreaseSpeed = 1.5;
	throttleLevel -= decreaseSpeed*deltaSeconds;
	if (throttleLevel < 0.) {
		throttleLevel = 0.;
	}
}

void Car::setBrake(float value) {
	assert(value >= 0.0 && value <= 1.0);
	brakeLevel = value;
}

float Car::getBrake() const {
	return brakeLevel;
}

void Car::increaseBrake(float deltaSeconds) {
	const float increaseSpeed = 0.9;
	brakeLevel += increaseSpeed*deltaSeconds;
	if (brakeLevel > 1.) {
		brakeLevel = 1.;
	}
}

void Car::decreaseBrake(float deltaSeconds) {
	const float decreaseSpeed = 1.5;
	brakeLevel -= decreaseSpeed*deltaSeconds;
	if (brakeLevel < 0.) {
		brakeLevel = 0.;
	}
}

void Car::increaseTurnToRight(float deltaSeconds) {
	const float turnSpeed = 1.5;
	turnLevel += deltaSeconds*turnSpeed;
	if (turnLevel > 1.) {
		turnLevel = 1.;
	}
}

void Car::increaseTurnToLeft(float deltaSeconds) {
	const float turnSpeed = 1.5;
	turnLevel -= deltaSeconds*turnSpeed;
	if (turnLevel < -1.) {
		turnLevel = -1.;
	}
}

void Car::dontTurn(float deltaSeconds) {
	const float turnSpeed = 1.5;
	if (turnLevel > 0.) {
		turnLevel -= deltaSeconds*turnSpeed;
		if (turnLevel < 0.) {
			turnLevel = 0.;
		}
	} else if (turnLevel < 0.) {
		turnLevel += deltaSeconds*turnSpeed;
		if (turnLevel > 0.) {
			turnLevel = 0.;
		}
	}
}

const sf::Vector2f& Car::getPosition() const {
	return position;
}

const sf::Vector2f& Car::getVelocity() const {
	return velocity;
}

const sf::Vector2f& Car::getOrientation() const {
	return orientation;
}

float Car::getSpeed() const {
	return getLength(velocity);
}

const sf::Vector2f& Car::getAcceleration() const {
	return acceleration;
}

}

