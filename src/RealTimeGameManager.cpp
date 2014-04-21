
#include "RealTimeGameManager.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cassert>

#include <boost/math/constants/constants.hpp>

#include "mathUtil.hpp"
#include "drawUtil.hpp"
#include "randomUtil.hpp"

namespace car {

RealTimeGameManager::RealTimeGameManager(std::function<Track()> trackCreator) :
	window(sf::VideoMode(800, 800), "car-game")
{

	using namespace boost::math::float_constants;


	Track track = trackCreator();
//	Track track = createCircleTrack();
	track.check();

	model.setCar(track.createCar());
	model.setTrack(track);

	font.loadFromFile("resources/DejaVuSansMono.ttf");
	gasTelemetry.setAutomaticBoundsDetection(false);
	gasTelemetry.setBounds(0.f, 1.f);
	brakeTelemetry.setAutomaticBoundsDetection(false);
	brakeTelemetry.setBounds(0.f, 1.f);
	angleTelemetry.setAutomaticBoundsDetection(false);
	angleTelemetry.setBounds(-pi, pi);
	turnTelemetry.setAutomaticBoundsDetection(false);
	turnTelemetry.setBounds(-1.f, 1.f);

	gameView.setCenter(0.f, 0.f);
	gameView.setSize(150.f, 150.f);
	hudView = window.getDefaultView();

}

void RealTimeGameManager::setFPSLimit(float newFpsLimit) {
	fpsLimit = newFpsLimit;
}

void RealTimeGameManager::run() {

	sf::Clock clock;

	float physicsTimeStepAccumulator = 0.f;
	while (window.isOpen()) {
		const sf::Time time = clock.restart();
		float deltaSeconds = time.asSeconds();
		fps = 1/deltaSeconds;

		//if we're really really slow
		if ( deltaSeconds > 0.1f ) {
			deltaSeconds = 0.1f;
		}
		physicsTimeStepAccumulator += deltaSeconds;
		while (physicsTimeStepAccumulator >= physicsTimeStep) {
			handleInput();
			model.advanceTime(physicsTimeStep);
			rayPoints = model.getRayPoints();
			physicsTimeStepAccumulator -= physicsTimeStep;
		}

		updateTelemetry();

		window.clear(sf::Color::Black);

		window.setView(gameView);
		model.draw(window);
		drawRayPoints();

		window.setView(hudView);
		drawTelemetry();

		window.display();
		if ( fpsLimit > 0 ) {
			const sf::Time renderTime = clock.getElapsedTime();
			if ( renderTime.asSeconds() < 1.f/fpsLimit ) {
				sf::sleep( sf::seconds(1.f/fpsLimit - renderTime.asSeconds()) );
			}
		}
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

	if ( isAIControl ) {
		Weights inputs;
		std::generate_n(std::back_inserter(inputs), 10, [] { return randomReal(0, 1); });

		Weights outputs = neuralNetwork.evaluateInput(inputs);
		assert(outputs.size() == 4);

		model.setForwardPressed(outputs[0] > 0.5);
		model.setBackwardPressed(outputs[1] > 0.5);
		model.setLeftPressed(outputs[2] > 0.5);
		model.setRightPressed(outputs[3] > 0.5);
	} else {
		model.setLeftPressed(pressedKeys[sf::Keyboard::Left]);
		model.setRightPressed(pressedKeys[sf::Keyboard::Right]);
		model.setForwardPressed(pressedKeys[sf::Keyboard::Up]);
		model.setBackwardPressed(pressedKeys[sf::Keyboard::Down]);
	}
}

void RealTimeGameManager::updateTelemetry() {
	float currentTime = model.getCurrentTime();
	const Car& car = model.getCar();

	speedTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getSpeed()));
	accelerationTelemetry.addDataPoint(sf::Vector2f(currentTime, getLength(car.getAcceleration())));
	angleTelemetry.addDataPoint(sf::Vector2f(currentTime, std::atan2(car.getOrientation().x, car.getOrientation().y)));
	gasTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getThrottle()));
	brakeTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getBrake()));
	turnTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getTurnLevel()));
}

void RealTimeGameManager::drawRayPoints() {
	const Car& car = model.getCar();

	for ( const auto& ray : rayPoints ) {
		if (!ray) {
			continue;
		}
		drawLine(window, car.getPosition(), *ray, sf::Color{64, 64, 0});
	}
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
		speedTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 600, 200), sf::Color::Green);
		accelerationTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 600, 200), sf::Color::Yellow);
		angleTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 600, 200), sf::Color::White);
		gasTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 600, 200), sf::Color::Red);
		brakeTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 600, 200), sf::Color::Magenta);
		turnTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 600, 200), sf::Color::Cyan);
	}
}

}

