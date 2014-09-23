
#include "Car.hpp"

#include <cassert>
#include <cmath>

#include <boost/math/constants/constants.hpp>

#include "drawUtil.hpp"
#include "mathUtil.hpp"

namespace car {

Car::Car(const CarParameters& parameters): parameters(parameters) {}

Car::Car(const CarParameters& parameters, const sf::Vector2f& position, float direction) :
		parameters(parameters), position(position),
		orientation(std::cos(direction), std::sin(direction)) {
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
	//float weight = parameters.mass * parameters.gravity;

	float power = parameters.pEngine * throttleLevel;

	float engineForce = std::max(0.f, std::min(power / speed, parameters.fEngineMax));
	float brakeForce = parameters.fBrake * brakeLevel;

	sf::Vector2f fTraction = velocityDirection * engineForce;
	sf::Vector2f fBraking = -velocityDirection * brakeForce;
	sf::Vector2f fDrag = -parameters.cDrag * velocity * speed;
	sf::Vector2f fRollingResistance = -parameters.cRollingResistance * velocity;

	sf::Vector2f fLongtitudinal = fTraction + fBraking + fDrag + fRollingResistance;

	acceleration = fLongtitudinal / parameters.mass;

	float wheelBase = parameters.wheelBase();
	//float weightFront = (parameters.rearWheelCMDistance / wheelBase)*weight -
		//(parameters.heightOfCG / wheelBase)*parameters.mass*(getLength(acceleration));
	//float weightRear = (parameters.frontWheelCMDistance / wheelBase)*weight -
		//(parameters.heightOfCG / wheelBase)*parameters.mass*(getLength(acceleration));
	//(void)weightFront;
	//(void)weightRear;

	velocity += deltaSeconds * acceleration;
	position += deltaSeconds * velocity;

	//We don't want anything accurate here
	travelDistance += deltaSeconds * speed;

	if (std::abs(turnLevel) > 0.0001) {
		float steeringAngle = parameters.maxTurnAngle * turnLevel;
		float turnRadius = wheelBase / std::sin(steeringAngle);

		float angularVelocity = parameters.turnRate / turnRadius;
		sf::Transform rotateTransform;
		rotateTransform.rotate(angularVelocity*deltaSeconds * 180.f/pi);
		orientation = rotateTransform.transformPoint(orientation);
	}

	updateCorners();
}

void Car::setThrottle(float value) {
	//assert(value >= 0.0 && value <= 1.0);
	throttleLevel = value;
}

float Car::getThrottle() const {
	return throttleLevel;
}

void Car::increaseThrottle(float deltaSeconds) {
	throttleLevel += parameters.throttleIncreaseSpeed * deltaSeconds;
	if (throttleLevel > 1.) {
		throttleLevel = 1.;
	}
}

void Car::decreaseThrottle(float deltaSeconds) {
	throttleLevel -= parameters.throttleDecreaseSpeed * deltaSeconds;
	if (throttleLevel < 0.) {
		throttleLevel = 0.;
	}
}

void Car::setBrake(float value) {
	//assert(value >= 0.0 && value <= 1.0);
	brakeLevel = value;
}

float Car::getBrake() const {
	return brakeLevel;
}

void Car::increaseBrake(float deltaSeconds) {
	brakeLevel += parameters.brakeIncreaseSpeed * deltaSeconds;
	if (brakeLevel > 1.f) {
		brakeLevel = 1.f;
	}
}

void Car::decreaseBrake(float deltaSeconds) {
	brakeLevel -= parameters.brakeDecreaseSpeed * deltaSeconds;
	if (brakeLevel < 0.f) {
		brakeLevel = 0.f;
	}
}

void Car::setTurnLevel(float value) {
	//assert(value >= -1.0 && value <= 1.0);
	turnLevel = value;
}

void Car::increaseTurnToRight(float deltaSeconds) {
	turnLevel += parameters.turnSpeed * deltaSeconds;
	if (turnLevel > 1.f) {
		turnLevel = 1.f;
	}
}

void Car::increaseTurnToLeft(float deltaSeconds) {
	turnLevel -= parameters.turnSpeed * deltaSeconds;
	if (turnLevel < -1.f) {
		turnLevel = -1.f;
	}
}

void Car::dontTurn(float deltaSeconds) {
	if (turnLevel > 0.f) {
		turnLevel -= parameters.turnSpeed * deltaSeconds;
		if (turnLevel < 0.f) {
			turnLevel = 0.f;
		}
	} else if (turnLevel < 0.f) {
		turnLevel += parameters.turnSpeed * deltaSeconds;
		if (turnLevel > 0.f) {
			turnLevel = 0.f;
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

	const float carHalfWidth = parameters.carWidth / 2.f;

	//CM is the origin when drawing
	frontLeftCorner = transform.transformPoint(
			sf::Vector2f(parameters.frontCMDistance, -carHalfWidth));
	frontRightCorner = transform.transformPoint(
			sf::Vector2f(parameters.frontCMDistance, carHalfWidth));
	rearLeftCorner = transform.transformPoint(
			sf::Vector2f(-parameters.rearCMDistance, -carHalfWidth));
	rearRightCorner = transform.transformPoint(
			sf::Vector2f(-parameters.rearCMDistance, carHalfWidth));
}

}

