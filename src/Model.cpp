
#include <assert.h>

#include <boost/math/constants/constants.hpp>

#include "Model.hpp"
#include "drawUtil.hpp"

namespace car {

Model::Model() {}

void Model::setCar(const Car& newCar) {
	car = newCar;
}

void Model::setTrack(const track::Track& newTrack) {
	track = newTrack;
}

const Car& Model::getCar() const {
	return car;
}

Car& Model::getCar() {
	return car;
}

const track::Track& Model::getTrack() const {
	return track;
}

track::Track& Model::getTrack() {
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

	const float maxViewDistance = 50.f;

	//right, (1, 0) is to the front
	std::vector<sf::Vector2f> directions(count);
	for (unsigned i = 0; i < count; ++i) {
		directions[i] = {1, i*4.f/count - 2.f};
	}

	//rotate them, so they align with the current rotation of the car
	const sf::Vector2f& carOrientation = car.getOrientation();

	sf::Transform transform;
	transform.rotate(std::atan2(carOrientation.y, carOrientation.x) * 180.f/pi);

	for ( sf::Vector2f& v : directions ) {
		v = transform.transformPoint(v);
	}

	std::vector<boost::optional<sf::Vector2f>> rayPoints;
	rayPoints.reserve(count);

	for ( const sf::Vector2f& v : directions ) {
		rayPoints.push_back(track.collideWithRay(car.getPosition(), v, maxViewDistance));
	}

	return rayPoints;
}

unsigned Model::getNumberOfCrossedCheckpoints() const {
	return numberOfCrossedCheckpoints;
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

bool Model::collidesWithCheckpoint(std::size_t checkpointId) {
	return track.collidesWithCheckpoint(
					Line2f(car.getFrontLeftCorner(), car.getFrontRightCorner()),
					checkpointId) ||
			track.collidesWithCheckpoint(
					Line2f(car.getFrontLeftCorner(), car.getRearLeftCorner()),
					checkpointId) ||
			track.collidesWithCheckpoint(
					Line2f(car.getFrontRightCorner(), car.getRearRightCorner()),
					checkpointId) ||
			track.collidesWithCheckpoint(
					Line2f(car.getRearLeftCorner(), car.getRearRightCorner()),
					checkpointId);
}

bool Model::findFirstCheckpoint() {
	for (std::size_t i = 0; i < track.getNumberOfCheckpoints(); ++i) {
		if (collidesWithCheckpoint(i)) {
			currentCheckpoint = (i + 1) % track.getNumberOfCheckpoints();
			++numberOfCrossedCheckpoints;
			return true;
		}
	}

	Line2f finderLine{car.getPosition(),
			track.collideWithRay(car.getPosition(), car.getOrientation(), 50.f)};
	for (std::size_t i = 0; i < track.getNumberOfCheckpoints(); ++i) {
		if (track.collidesWithCheckpoint(finderLine, i)) {
			currentCheckpoint = i;
			return true;
		}
	}

	return false;
}

void Model::handleCheckpoints() {
	if (currentCheckpoint < 0) {
		if (!findFirstCheckpoint()) {
			return;
		}
	} else {
		if (collidesWithCheckpoint(currentCheckpoint)) {
			currentCheckpoint = (currentCheckpoint + 1) % track.getNumberOfCheckpoints();
			++numberOfCrossedCheckpoints;
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

void Model::drawCar(sf::RenderWindow& window) const {
	car.draw(window);
}

void Model::drawTrack(sf::RenderWindow& window, bool drawCheckpoints) const {
	track.drawBoundary(window);
	if (drawCheckpoints) {
		track.drawCheckpoints(window, currentCheckpoint);
	}
}

sf::Vector2f Model::getCheckpointDirection() const {
	using namespace boost::math::float_constants;
	if (currentCheckpoint < 0) {
		return {};
	}
	const auto& position = car.getPosition();
	const auto& orientation = car.getOrientation();
	auto angle = std::atan2(orientation.y, orientation.x);
	auto nearestPointToCheckpoint = nearestPoint(position, track.getCheckpoint(currentCheckpoint));
	auto absoluteDirection = nearestPointToCheckpoint - position;
	sf::Transform rotateTransform;
	rotateTransform.rotate(-angle * 180.f/pi);
	auto relativeDirection = rotateTransform.transformPoint(absoluteDirection);

	return normalize(relativeDirection);
}


}

