
#include "Car.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "util.hpp"

namespace car {

Car::Car(const sf::Vector2f& position) : position(position) {}

void Car::move(float deltaSeconds) {

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

	float weightFront = (rearWheelCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(getLength(acceleration));
	float weightRear = (frontWheelCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(getLength(acceleration));

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
	throttleLevel += throttleIncreaseSpeed*deltaSeconds;
	if (throttleLevel > 1.) {
		throttleLevel = 1.;
	}
}

void Car::decreaseThrottle(float deltaSeconds) {
	throttleLevel -= throttleDecreaseSpeed*deltaSeconds;
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
	brakeLevel += brakeIncreaseSpeed*deltaSeconds;
	if (brakeLevel > 1.) {
		brakeLevel = 1.;
	}
}

void Car::decreaseBrake(float deltaSeconds) {
	brakeLevel -= brakeDecreaseSpeed*deltaSeconds;
	if (brakeLevel < 0.) {
		brakeLevel = 0.;
	}
}

void Car::increaseTurnToRight(float deltaSeconds) {
	turnLevel += turnSpeed*deltaSeconds;
	if (turnLevel > 1.) {
		turnLevel = 1.;
	}
}

void Car::increaseTurnToLeft(float deltaSeconds) {
	turnLevel -= turnSpeed*deltaSeconds;
	if (turnLevel < -1.) {
		turnLevel = -1.;
	}
}

void Car::dontTurn(float deltaSeconds) {
	if (turnLevel > 0.) {
		turnLevel -= turnSpeed*deltaSeconds;
		if (turnLevel < 0.) {
			turnLevel = 0.;
		}
	} else if (turnLevel < 0.) {
		turnLevel += turnSpeed*deltaSeconds;
		if (turnLevel > 0.) {
			turnLevel = 0.;
		}
	}
}

void Car::draw(sf::RenderWindow& window) const {
	sf::Transform transform;
	transform.translate(getPosition());
	transform.rotate(std::atan2(getOrientation().y, getOrientation().x) * 180.f/M_PI);

	const float carHalfWidth = carWidth/2.f;

	//CM is the origin when drawing
	sf::Vector2f frontLeft = transform.transformPoint(sf::Vector2f(frontCMDistance, -carHalfWidth));
	sf::Vector2f frontRight = transform.transformPoint(sf::Vector2f(frontCMDistance, carHalfWidth));
	sf::Vector2f rearLeft = transform.transformPoint(sf::Vector2f(-rearCMDistance, -carHalfWidth));
	sf::Vector2f rearRight = transform.transformPoint(sf::Vector2f(-rearCMDistance, carHalfWidth));

	drawLine(window, frontLeft, frontRight);
	drawLine(window, frontLeft, rearLeft);
	drawLine(window, frontRight, rearRight);
	drawLine(window, rearLeft, rearRight);
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

