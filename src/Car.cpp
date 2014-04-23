
#include "Car.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include <boost/math/constants/constants.hpp>

#include "drawUtil.hpp"
#include "mathUtil.hpp"

namespace car {

const float Car::cDrag = 0.5;
const float Car::cRollingResistance = 14.2;
const float Car::pEngine = 40000.0;
const float Car::fEngineMax = 10000.0;
const float Car::fBrake = 30000.0;
const float Car::gravity = 9.8; //m/s^2
const float Car::transMissionEfficiency = 0.7;
const float Car::gearRatio = 2.1;
const float Car::differentialRatio = 3.42;
const float Car::wheelRadius = 0.34; //m
const float Car::mass = 1500; //kg
const float Car::maxTurnAngle = 0.52; //radians ~= 30 degrees
const float Car::turnRate = 8.0;

	//CM == Center of Mass
	//CG == Center of Gravity
const float Car::rearWheelCMDistance = 1.0;
const float Car::frontWheelCMDistance = 1.5;
const float Car::rearCMDistance = 1.2;
const float Car::frontCMDistance = 1.8;
const float Car::heightOfCG = 1.5;
const float Car::wheelBase = rearCMDistance + frontCMDistance;
const float Car::carWidth = 1.4;
const float Car::throttleIncreaseSpeed = 1.5;
const float Car::throttleDecreaseSpeed = 1.3;
const float Car::brakeIncreaseSpeed = 1.9;
const float Car::brakeDecreaseSpeed = 1.7;
const float Car::turnSpeed = 6.5;

Car::Car() {}

Car::Car(const sf::Vector2f& position, float direction) :
		position(position), orientation(std::cos(direction), std::sin(direction)) {
	updateCorners();
}

void Car::setColor(const sf::Color& newColor) {
	color = newColor;
}

void Car::move(float deltaSeconds) {

	using namespace boost::math::float_constants;

	sf::Vector2f velocityDirection = orientation;

	velocity = orientation*getLength(velocity); //hack, TODO something

	float speed = getSpeed();
	float weight = mass * gravity;

	float power = pEngine * throttleLevel;

	float engineForce = std::max(0.f, std::min(power / speed, fEngineMax));
	float brakeForce = fBrake * brakeLevel;

	sf::Vector2f fTraction = velocityDirection * engineForce;
	sf::Vector2f fBraking = -velocityDirection * brakeForce;
	sf::Vector2f fDrag = -cDrag * velocity * speed;
	sf::Vector2f fRollingResistance = -cRollingResistance * velocity;

	sf::Vector2f fLongtitudinal = fTraction + fBraking + fDrag + fRollingResistance;

	acceleration = fLongtitudinal / mass;

	float weightFront = (rearWheelCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(getLength(acceleration));
	float weightRear = (frontWheelCMDistance / wheelBase)*weight - (heightOfCG / wheelBase)*mass*(getLength(acceleration));
	(void)weightFront;
	(void)weightRear;

	velocity += deltaSeconds * acceleration;
	position += deltaSeconds * velocity;

	//We don't want anything accurate here
	travelDistance += deltaSeconds * speed;

	if (std::abs(turnLevel) > 0.0001) {
		float steeringAngle = maxTurnAngle * turnLevel;
		float turnRadius = wheelBase / std::sin(steeringAngle);

		float angularVelocity = turnRate / turnRadius;
		sf::Transform rotateTransform;
		rotateTransform.rotate(angularVelocity*deltaSeconds * 180.f/pi);
		orientation = rotateTransform.transformPoint(orientation);
	}

	updateCorners();
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

void Car::setTurnLevel(float value) {
	assert(value >= -1.0 && value <= 1.0);
	turnLevel = value;
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

const sf::Vector2f& Car::getFrontLeftCorner() const {
	return frontLeftCorner;
}

const sf::Vector2f& Car::getFrontRightCorner() const {
	return frontRightCorner;
}

const sf::Vector2f& Car::getRearLeftCorner() const {
	return rearLeftCorner;
}

const sf::Vector2f& Car::getRearRightCorner() const {
	return rearRightCorner;
}

void Car::draw(sf::RenderWindow& window) const {
	drawLine(window, frontLeftCorner, frontRightCorner, color);
	drawLine(window, frontLeftCorner, rearLeftCorner, color);
	drawLine(window, frontRightCorner, rearRightCorner, color);
	drawLine(window, rearLeftCorner, rearRightCorner, color);
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

float Car::getTurnLevel() const {
	return turnLevel;
}

float Car::getSpeed() const {
	return getLength(velocity);
}

const sf::Vector2f& Car::getAcceleration() const {
	return acceleration;
}

float Car::getTravelDistance() const {
	return travelDistance;
}

void Car::updateCorners() {

	using namespace boost::math::float_constants;

	sf::Transform transform;
	transform.translate(getPosition());
	transform.rotate(std::atan2(getOrientation().y, getOrientation().x) * 180.f/pi);

	const float carHalfWidth = carWidth/2.f;

	//CM is the origin when drawing
	frontLeftCorner = transform.transformPoint(sf::Vector2f(frontCMDistance, -carHalfWidth));
	frontRightCorner = transform.transformPoint(sf::Vector2f(frontCMDistance, carHalfWidth));
	rearLeftCorner = transform.transformPoint(sf::Vector2f(-rearCMDistance, -carHalfWidth));
	rearRightCorner = transform.transformPoint(sf::Vector2f(-rearCMDistance, carHalfWidth));
}

}

