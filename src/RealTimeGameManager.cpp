
#include "RealTimeGameManager.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cassert>

#include <boost/math/constants/constants.hpp>

#include "mathUtil.hpp"
#include "drawUtil.hpp"
#include "randomUtil.hpp"
#include "ScreenDimension.hpp"

namespace car {

auto RealTimeGameManager::createCarData(const CommonParameters& parameters, track::TrackCreator trackCreator) -> CarData {
	using namespace boost::math::float_constants;

	CarData result{parameters, trackCreator};

	result.gasTelemetry.setAutomaticBoundsDetection(false);
	result.gasTelemetry.setBounds(0.f, 1.f);
	result.brakeTelemetry.setAutomaticBoundsDetection(false);
	result.brakeTelemetry.setBounds(0.f, 1.f);
	result.angleTelemetry.setAutomaticBoundsDetection(false);
	result.angleTelemetry.setBounds(-pi, pi);
	result.turnTelemetry.setAutomaticBoundsDetection(false);
	result.turnTelemetry.setBounds(-1.f, 1.f);

	return result;
}

RealTimeGameManager::RealTimeGameManager(const RealTimeParameters& realTimeParameters, track::TrackCreator trackCreator) :
	window(sf::VideoMode(
				realTimeParameters.screenWidth,
				realTimeParameters.screenHeight), "car-game"),
	realTimeParameters(realTimeParameters),
	physicsTimeStep(getPhysicsTimeStep(realTimeParameters.commonParameters))
{

	setFPSLimit(realTimeParameters.fpsLimit);

	font.loadFromFile(realTimeParameters.projectRootPath + "/resources/DejaVuSansMono.ttf");

	hudView = window.getDefaultView();

	if (realTimeParameters.carInputParameters.neuralNetworkFile.empty()) {
		carDatas.push_back(createCarData(realTimeParameters.commonParameters, trackCreator));
		carDatas.back().gameManager.setIsAIControl(false);
	} else {
		for (const auto& neuralNetworkFile: realTimeParameters.carInputParameters.neuralNetworkFile) {
			carDatas.push_back(createCarData(realTimeParameters.commonParameters, trackCreator));
			auto& carData = carDatas.back();
			auto& gameManager = carData.gameManager;
			gameManager.setIsAIControl(true);
			gameManager.setNeuralNetwork(loadNeuralNetworkFromFile(neuralNetworkFile));
			carData.name = neuralNetworkFile;
		}
	}
}

void RealTimeGameManager::run() {

	sf::Clock clock;

	float physicsTimeStepAccumulator = 0.f;

	while (window.isOpen()) {
		const sf::Time time = clock.restart();
		float deltaSeconds = time.asSeconds();
		fps = 1/deltaSeconds;

		//if we're really really slow
		if (deltaSeconds > 0.1f) {
			deltaSeconds = 0.1f;
		}
		physicsTimeStepAccumulator += deltaSeconds;
		while (physicsTimeStepAccumulator >= physicsTimeStep) {
			handleUserInput();
			for (auto& carData: carDatas) {
				carData.gameManager.advance();
			}
			physicsTimeStepAccumulator -= physicsTimeStep;
		}

		updateTelemetry();

		setViewParameters();
		window.clear(sf::Color::Black);

		window.setView(gameView);
		drawGame();

		window.setView(hudView);
		drawTelemetry();

		window.display();
		if (fpsLimit > 0) {
			const sf::Time renderTime = clock.getElapsedTime();
			if (renderTime.asSeconds() < 1.f/fpsLimit) {
				sf::sleep( sf::seconds(1.f/fpsLimit - renderTime.asSeconds()) );
			}
		}
	}
}

float RealTimeGameManager::calculateCenter(float viewSize, float trackOrigin, float trackSize, float carPosition) {
	switch (realTimeParameters.panMode) {
		case PanMode::center:
			return carPosition;
		case PanMode::fit:
			{
				if (trackSize < viewSize) {
					return trackOrigin + trackSize / 2.f;
				}

				const auto viewHalfSize = viewSize / 2.f;

				if (carPosition - viewHalfSize < trackOrigin) {
					return trackOrigin + viewHalfSize;
				}

				auto trackEnd = trackOrigin + trackSize;
				if (carPosition + viewHalfSize > trackEnd) {
					return trackEnd - viewHalfSize;
				}

				return carPosition;
			}
	}
}

void RealTimeGameManager::setViewParameters() {
	auto& gameManager = carDatas[currentCarId].gameManager;
	auto screenSize = window.getSize();
	auto trackDimensions = gameManager.getTrack().getDimensions();
	auto maxViewSize = sf::Vector2f{screenSize.x / realTimeParameters.minPixelsPerMeter, screenSize.y / realTimeParameters.minPixelsPerMeter};
	auto minViewSize = sf::Vector2f{screenSize.x / realTimeParameters.maxPixelsPerMeter, screenSize.y / realTimeParameters.maxPixelsPerMeter};

	auto fitViewSize1 = sf::Vector2f{
			trackDimensions.width,
			trackDimensions.width * screenSize.y / screenSize.x};
	auto fitViewSize2 = sf::Vector2f{
			trackDimensions.height * screenSize.x / screenSize.y,
			trackDimensions.height};

	if (fitViewSize2.x < fitViewSize1.x) {
		std::swap(fitViewSize1, fitViewSize2);
	}

	sf::Vector2f viewSize;

	if (fitViewSize2.x < minViewSize.x) {
		viewSize = minViewSize;
	} else if (fitViewSize2.x > maxViewSize.x) {
		viewSize = maxViewSize;
	} else {
		viewSize = fitViewSize2;
	}

	pixelsPerMeter = screenSize.x / viewSize.x;

	gameView.setSize(viewSize.x, viewSize.y);

	auto& carPosition = gameManager.getModel().getCar().getPosition();
	auto& viewCenter = gameView.getCenter();
	panThreshold =  boost::apply_visitor(
				ScreenDimensionConverter{viewSize, pixelsPerMeter},
				realTimeParameters.panThreshold);

	if (getDistance(carPosition, viewCenter) > panThreshold) {
		auto preferredCenter = carPosition + normalize(viewCenter - carPosition) * panThreshold;
		gameView.setCenter(
				calculateCenter(viewSize.x, trackDimensions.left, trackDimensions.width,
					preferredCenter.x),
				calculateCenter(viewSize.y, trackDimensions.top, trackDimensions.height,
					preferredCenter.y));
	}


	hudView.reset(sf::FloatRect(0.f, 0.f, screenSize.x, screenSize.y));
}

void RealTimeGameManager::setFPSLimit(float newFPSLimit) {
	fpsLimit = newFPSLimit;
}

void RealTimeGameManager::handleUserInput() {
	auto& gameManager = carDatas[currentCarId].gameManager;
	sf::Event event;

	while (window.pollEvent(event)) {
		switch(event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			pressedKeys[event.key.code] = true;
			switch(event.key.code) {
			case sf::Keyboard::C:
				showCar = !showCar;
				break;
			case sf::Keyboard::Y:
				showRays = !showRays;
				break;
			case sf::Keyboard::P:
				showCheckPoints = !showCheckPoints;
				break;
			case sf::Keyboard::R:
				showTrackBoundary = !showTrackBoundary;
				break;
			case sf::Keyboard::T:
				showTelemetryGraphs = !showTelemetryGraphs;
				break;
			case sf::Keyboard::X:
				showTelemetryText = !showTelemetryText;
				break;
			case sf::Keyboard::A:
				gameManager.setIsAIControl(!gameManager.getIsAIControl());
				break;
			case sf::Keyboard::PageDown:
				++currentCarId;
				currentCarId %= carDatas.size();
				break;
			case sf::Keyboard::PageUp:
				if (currentCarId == 0) {
					currentCarId = carDatas.size() - 1;
				} else {
					--currentCarId;
				}
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

	if (!gameManager.getIsAIControl()) {
		auto& model = gameManager.getModel();
		model.setLeftPressed(pressedKeys[sf::Keyboard::Left]);
		model.setRightPressed(pressedKeys[sf::Keyboard::Right]);
		model.setForwardPressed(pressedKeys[sf::Keyboard::Up]);
		model.setBackwardPressed(pressedKeys[sf::Keyboard::Down]);
	}
}

void RealTimeGameManager::updateTelemetry() {
	for (auto& carData: carDatas) {
		auto& gameManager = carData.gameManager;
		auto& model = gameManager.getModel();

		float currentTime = model.getCurrentTime();
		const Car& car = model.getCar();

		carData.speedTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getSpeed()));
		carData.accelerationTelemetry.addDataPoint(sf::Vector2f(currentTime, getLength(car.getAcceleration())));
		carData.angleTelemetry.addDataPoint(sf::Vector2f(currentTime, std::atan2(car.getOrientation().x, car.getOrientation().y)));
		carData.gasTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getThrottle()));
		carData.brakeTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getBrake()));
		carData.turnTelemetry.addDataPoint(sf::Vector2f(currentTime, car.getTurnLevel()));
	}
}

void RealTimeGameManager::drawGame() {
	if (showTrackBoundary) {
		carDatas[currentCarId].gameManager.getModel().drawTrack(window, showCheckPoints);
	}
	if (showRays) {
		drawRays();
	}
	if (showCar) {
		for (const auto& carData: carDatas) {
			carData.gameManager.getModel().drawCar(window);
		}
	}

	//auto circle = sf::CircleShape{panThreshold};
	//auto center = gameView.getCenter();
	//circle.setOrigin(panThreshold, panThreshold);
	//circle.setPosition(center);
	//circle.setFillColor(sf::Color::Transparent);
	//circle.setOutlineColor(sf::Color::White);
	//circle.setOutlineThickness(0.1);
	//window.draw(circle);

}

void RealTimeGameManager::drawRays() {
	auto& gameManager = carDatas[currentCarId].gameManager;
	const Car& car = gameManager.getModel().getCar();

	for (const auto& ray : gameManager.getRayPoints()) {
		if (!ray) {
			continue;
		}
		drawLine(window, car.getPosition(), *ray, sf::Color{64, 64, 0});
	}
}

void RealTimeGameManager::drawTelemetry() {
	auto& carData = carDatas[currentCarId];
	auto& gameManager = carData.gameManager;
	using namespace boost::math::float_constants;

	auto& model = gameManager.getModel();

	if (showTelemetryText) {
		const Car& car = model.getCar();

		auto checkpointDirection = model.getCheckpointDirection();

		std::stringstream ss;
		ss << std::fixed <<
			"FPS = " << std::setw(4) << std::setfill('0') << static_cast<int>(fps) <<
			", Speed = " << car.getSpeed() <<
			", Acceleration = " << getLength(car.getAcceleration()) <<
			", Throttle = " << car.getThrottle() <<
			",\nBrake = " << car.getBrake() <<
			", Checkpoint = (" << checkpointDirection.x << ", " << checkpointDirection.y << ")" <<
			", TravelDistance = " << car.getTravelDistance() <<
			",\nppm = " << pixelsPerMeter;
		if (!carData.name.empty()) {
			ss << ",AI = " << carData.name;
		}
		sf::Text text;
		text.setFont(font);
		text.setColor(sf::Color::White);
		text.setCharacterSize(32);
		text.setString(sf::String(ss.str()));
		text.setScale(.5, .5);
		text.setPosition(3., 3.);

		window.draw(text);
	}

	if (showTelemetryGraphs) {
		carData.speedTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 600, 200), sf::Color::Green);
		carData.accelerationTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 600, 200), sf::Color::Yellow);
		carData.angleTelemetry.drawAsGraph(window, sf::FloatRect(10, 20, 600, 200), sf::Color::White);
		carData.gasTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 600, 200), sf::Color::Red);
		carData.brakeTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 600, 200), sf::Color::Magenta);
		carData.turnTelemetry.drawAsGraph(window, sf::FloatRect(10, 230, 600, 200), sf::Color::Cyan);
	}
}

}

