
#include "RealTimeGameManager.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "mathUtil.hpp"

namespace car {

RealTimeGameManager::RealTimeGameManager() :
	window(sf::VideoMode(1024, 1024), "car-game")
{

	model.setCar(Car(sf::Vector2f(0, 55)));
	{
		Track track;
		const int circleResolution = 20;
		const float innerCircleRadius = 50.;
		const float outerCircleRadius = 60.;
		for ( int i = 0; i < circleResolution; ++i ) {
			track.addLine(Line2f(
						innerCircleRadius*std::cos((i-1)*2*M_PI/circleResolution),
						innerCircleRadius*std::sin((i-1)*2*M_PI/circleResolution),
						innerCircleRadius*std::cos((i)*2*M_PI/circleResolution),
						innerCircleRadius*std::sin((i)*2*M_PI/circleResolution)));
			track.addLine(Line2f(
						outerCircleRadius*std::cos((i-1)*2*M_PI/circleResolution),
						outerCircleRadius*std::sin((i-1)*2*M_PI/circleResolution),
						outerCircleRadius*std::cos((i)*2*M_PI/circleResolution),
						outerCircleRadius*std::sin((i)*2*M_PI/circleResolution)));
		}
		model.setTrack(track);
	}

	font.loadFromFile("resources/DejaVuSansMono.ttf");
	gasTelemetry.setAutomaticBoundsDetection(false);
	gasTelemetry.setBounds(0.f, 1.f);
	brakeTelemetry.setAutomaticBoundsDetection(false);
	brakeTelemetry.setBounds(0.f, 1.f);

	gameView.setCenter(0.f, 0.f);
	gameView.setSize(150.f, 150.f);
	hudView = window.getDefaultView();

}

void RealTimeGameManager::run() {

	sf::Clock clock;

	float physicsTimeStepAccumulator = 0.f;
	while (window.isOpen()) {
		const sf::Time time = clock.restart();
		float deltaSeconds = time.asSeconds();
		fps = 1/deltaSeconds;

		handleInput();

		//if we're really really slow
		if ( deltaSeconds > 0.1f ) {
			deltaSeconds = 0.1f;
		}
		physicsTimeStepAccumulator += deltaSeconds;
		while (physicsTimeStepAccumulator >= physicsTimeStep) {
			model.advanceTime(physicsTimeStep);
			physicsTimeStepAccumulator -= physicsTimeStep;
		}

		updateTelemetry();

		window.clear(sf::Color::Black);

		window.setView(gameView);
		model.getTrack().draw(window);
		model.getCar().draw(window);

		window.setView(hudView);
		drawTelemetry();

		window.display();
	}
}

void RealTimeGameManager::handleInput() {

	sf::Event event;
	while (window.pollEvent(event)) {
		switch(event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			pressedKeys[event.key.code] = true;
			switch(event.key.code) {
			case sf::Keyboard::T:
				showTelemetry = !showTelemetry;
				break;
			default:
				break;
			}
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

	model.setLeftPressed(pressedKeys[sf::Keyboard::Left]);
	model.setRightPressed(pressedKeys[sf::Keyboard::Right]);
	model.setForwardPressed(pressedKeys[sf::Keyboard::Up]);
	model.setBackwardPressed(pressedKeys[sf::Keyboard::Down]);
}

void RealTimeGameManager::updateTelemetry() {
	float currentTime = model.getCurrentTime();
	const Car& car = model.getCar();

	speedTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getSpeed()));
	accelerationTelemetry.addDataPoint(sf::Vector2f(currentTime, getLength(car.getAcceleration())));
	gasTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getThrottle()));
	brakeTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getBrake()));
}

void RealTimeGameManager::drawTelemetry() {
	const Car& car = model.getCar();

	std::stringstream ss;
   	ss << std::fixed <<
	   	"FPS = " << std::setw(4) << std::setfill('0') << static_cast<int>(fps) <<
		", Speed = " << car.getSpeed() <<
		", Acceleration = " << getLength(car.getAcceleration()) <<
		", Throttle = " << car.getThrottle() <<
		", Brake = " << car.getBrake();
	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::White);
	text.setCharacterSize(32);
	text.setString(sf::String(ss.str()));
	text.setScale(.5, .5);
	text.setPosition(3., 3.);

	window.draw(text);

	if (showTelemetry) {
		speedTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 1004, 200));
		accelerationTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 1004, 200));
		gasTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 1004, 200));
		brakeTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 1004, 200));
	}
}

}

