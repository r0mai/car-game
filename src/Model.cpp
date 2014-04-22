
#include <assert.h>

#include <boost/math/constants/constants.hpp>

#include "Model.hpp"

namespace car {

Model::Model() {}

void Model::setCar(const Car& newCar) {
	car = newCar;
}

void Model::setTrack(const Track& newTrack) {
	track = newTrack;
}

const Car& Model::getCar() const {
	return car;
}

Car& Model::getCar() {
	return car;
}

const Track& Model::getTrack() const {
	return track;
}

Track& Model::getTrack() {
	return track;
}

void Model::setRightPressed(bool isPressed) {
	rightPressed = isPressed;
}

void Model::setLeftPressed(bool isPressed) {
	leftPressed = isPressed;
}

void Model::setForwardPressed(bool isPressed) {
	forwardPressed = isPressed;
}

void Model::setBackwardPressed(bool isPressed) {
	backwardPressed = isPressed;
}

float Model::getCurrentTime() const {
	return currentTime;
}

bool Model::hasCarCollided() const {
	return isCarCollided;
}

std::vector<boost::optional<sf::Vector2f>> Model::getRayPoints(unsigned count) const {

	using namespace boost::math::float_constants;

	//right, (1, 0) is to the front
	std::vector<sf::Vector2f> directions(count);
	for (unsigned i = 0; i < count; ++i) {
		directions[i] = {1, i*2.f/count - 1.f};
	}

	//rotate them, so they align with the current rotation of the car
	const sf::Vector2f& carOrientation = car.getOrientation();

	sf::Transform transform;
	transform.rotate(std::atan2(carOrientation.y, carOrientation.x) * 180.f/pi);

	for ( sf::Vector2f& v : directions ) {
		v = transform.transformPoint(v);
	}

	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	for ( const sf::Vector2f& v : directions ) {
		rayPoints.push_back(track.collideWithRay(car.getPosition(), v));
	}

	return rayPoints;
}

void Model::advanceTime(float deltaSeconds) {
	currentTime += deltaSeconds;

	handleInput(deltaSeconds);

	car.move(deltaSeconds);
	collideCar();
	handleCheckpoints();
}

void Model::collideCar() {
	isCarCollided =
		track.collidesWith(Line2f(car.getFrontLeftCorner(), car.getFrontRightCorner())) ||
		track.collidesWith(Line2f(car.getFrontLeftCorner(), car.getRearLeftCorner())) ||
		track.collidesWith(Line2f(car.getFrontRightCorner(), car.getRearRightCorner())) ||
		track.collidesWith(Line2f(car.getRearLeftCorner(), car.getRearRightCorner()));
	
	if (isCarCollided) {	
		car.setColor(sf::Color::Red);
	} else {
		car.setColor(sf::Color::White);
	}
}

void Model::handleCheckpoints() {
	int checkpoint = -1;
	if (    (checkpoint = track.checkpointCollidesWith(
					Line2f(car.getFrontLeftCorner(), car.getFrontRightCorner()))) >= 0 ||
			(checkpoint = track.checkpointCollidesWith(
					Line2f(car.getFrontLeftCorner(), car.getRearLeftCorner()))) >= 0 ||
			(checkpoint = track.checkpointCollidesWith(
					Line2f(car.getFrontRightCorner(), car.getRearRightCorner()))) >= 0 ||
			(checkpoint = track.checkpointCollidesWith(
					Line2f(car.getRearLeftCorner(), car.getRearRightCorner()))) >= 0
	) {
		assert(checkpoint >= 0);

		if (currentCheckpoint < 0 || currentCheckpoint == checkpoint) {
			currentCheckpoint = (checkpoint + 1) % track.getNumberOfCheckpoints();
		}
	}
}

void Model::handleInput(float deltaSeconds) {
	if (forwardPressed) {
		car.increaseThrottle(deltaSeconds);
	} else {
		car.decreaseThrottle(deltaSeconds);
	}
	if (backwardPressed) {
		car.increaseBrake(deltaSeconds);
	} else {
		car.decreaseBrake(deltaSeconds);
	}
	if (!leftPressed && !rightPressed) {
		car.dontTurn(deltaSeconds);
	} else {
		if (leftPressed) {
			car.increaseTurnToLeft(deltaSeconds);
		} else if (rightPressed) {
			car.increaseTurnToRight(deltaSeconds);
		}
	}

}

void Model::draw(sf::RenderWindow& window) const {
	track.draw(window, currentCheckpoint);
	car.draw(window);
}

}

