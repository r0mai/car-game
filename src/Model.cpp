
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

const Track& Model::getTrack() const {
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

void Model::advanceTime(float deltaSeconds) {
	currentTime += deltaSeconds;

	handleInput(deltaSeconds);

	car.move(deltaSeconds);
	collideCar();
}

void Model::collideCar() {
	if (    track.collidesWith(Line2f(car.getFrontLeftCorner(), car.getFrontRightCorner())) ||
			track.collidesWith(Line2f(car.getFrontLeftCorner(), car.getRearLeftCorner())) ||
			track.collidesWith(Line2f(car.getFrontRightCorner(), car.getRearRightCorner())) ||
			track.collidesWith(Line2f(car.getRearLeftCorner(), car.getRearRightCorner()))
	) {
		car.setColor(sf::Color::Red);
	} else {
		car.setColor(sf::Color::White);
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

}

