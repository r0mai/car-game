
#include "GameManager.hpp"

#include <iostream>
#include <sstream>

#include "util.hpp"

namespace car {

GameManager::GameManager() :
	window(sf::VideoMode(1024, 1024), "car-game"),
	car()
{
	font.loadFromFile("models/arial.ttf");
	gasTelemetry.setAutomaticBoundsDetection(false);
	gasTelemetry.setBounds(0.f, 1.f);
	brakeTelemetry.setAutomaticBoundsDetection(false);
	brakeTelemetry.setBounds(0.f, 1.f);
}

void GameManager::run() {


	sf::View view;
	view.setCenter(0.f, 0.f);
	view.setSize(400.f, 400.f);

	window.setView(view);

	sf::Clock clock;

	while(window.isOpen()) {
		const sf::Time time = clock.restart();
		const float deltaSeconds = time.asSeconds();

		currentTime += deltaSeconds;

		handleInput(deltaSeconds);

		car.move(deltaSeconds);
		updateTelemetry();

		window.clear(sf::Color::Black);

		drawCar();
		drawTelemetry();

		window.display();
	}
}

void GameManager::handleInput(float deltaSeconds) {

	sf::Event event;
	while (window.pollEvent(event)) {
		switch(event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			pressedKeys[event.key.code] = true;
			break;
		case sf::Event::KeyReleased:
			pressedKeys[event.key.code] = false;
			break;
		default:
			break;
		}
	}

	if (pressedKeys[sf::Keyboard::Q] || pressedKeys[sf::Keyboard::Escape]) {
		window.close();
	}
	if (pressedKeys[sf::Keyboard::Up]) {
		car.increaseThrottle(deltaSeconds);
	} else {
		car.decreaseThrottle(deltaSeconds);
	}
	if (pressedKeys[sf::Keyboard::Down]) {
		car.increaseBrake(deltaSeconds);
	} else {
		car.decreaseBrake(deltaSeconds);
	}
	if (!pressedKeys[sf::Keyboard::Left] && !pressedKeys[sf::Keyboard::Right]) {
		car.dontTurn(deltaSeconds);
	} else {
		if (pressedKeys[sf::Keyboard::Left]) {
			car.increaseTurnToLeft(deltaSeconds);
		} else if (pressedKeys[sf::Keyboard::Right]) {
			car.increaseTurnToRight(deltaSeconds);
		}
	}
}

void GameManager::drawCar() {
	sf::Vector2f topLeft(-2.5, -2.5);
	sf::Vector2f topRight(2.5, -2.5);
	sf::Vector2f bottomLeft(-2.5, 2.5);
	sf::Vector2f bottomRight(2.5, 2.5);

	topLeft += car.getPosition();
	topRight += car.getPosition();
	bottomLeft += car.getPosition();
	bottomRight += car.getPosition();

	drawLine(window, topLeft, topRight);
	drawLine(window, topLeft, bottomLeft);
	drawLine(window, topRight, bottomRight);
	drawLine(window, bottomLeft, bottomRight);
}

void GameManager::updateTelemetry() {
	speedTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getSpeed()));
	accelerationTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getAcceleration().y));
	gasTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getThrottle()));
	brakeTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getBrake()));
}

void GameManager::drawTelemetry() {
	std::stringstream ss;
   	ss << std::fixed << "Speed = " << car.getSpeed() << ", Acceleration = " << car.getAcceleration().y << ", Throttle = " << car.getThrottle() << ", Brake = " << car.getBrake();
	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::White);
	text.setCharacterSize(8);
	text.setString(sf::String(ss.str()));
	text.setPosition(window.mapPixelToCoords(sf::Vector2i(5, 5)));

	window.draw(text);

	speedTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 1004, 200));
	accelerationTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 1004, 200));
	gasTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 1004, 200));
	brakeTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 1004, 200));
}

}

