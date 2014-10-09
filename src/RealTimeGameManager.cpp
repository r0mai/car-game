
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

const float RealTimeGameManager::areaGridDistance = 2.f;
const float RealTimeGameManager::areaGridPointSize = 0.1f;
const sf::Color RealTimeGameManager::trackEdgeColor = sf::Color::White;
const sf::Color RealTimeGameManager::trackCenterLineColor{64, 64, 64};
const sf::Color RealTimeGameManager::checkpointColor{128, 128, 255};
const sf::Color RealTimeGameManager::checkpointNextColor{255, 255, 64};
const sf::Color RealTimeGameManager::checkpointLookaheadColor{128, 128, 60};
const sf::Color RealTimeGameManager::carNormalColor = sf::Color::White;
const sf::Color RealTimeGameManager::carActiveColor = sf::Color::Green;
const sf::Color RealTimeGameManager::carOutColor = sf::Color::Red;
const sf::Color RealTimeGameManager::carOutTimeColor{160, 0, 0};
const sf::Color RealTimeGameManager::traceColor = sf::Color::Magenta;

auto RealTimeGameManager::createCarData(const CommonParameters& parameters,
		std::shared_ptr<const track::Track> track) -> CarData {
	using namespace boost::math::float_constants;

	CarData result{parameters, std::move(track)};

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

RealTimeGameManager::RealTimeGameManager(const RealTimeParameters& realTimeParameters,
		std::shared_ptr<const track::Track> track) :
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
		carDatas.push_back(createCarData(realTimeParameters.commonParameters, track));
		carDatas.back().gameManager.setIsAIControl(false);
	} else {
		for (const auto& neuralNetworkFile: realTimeParameters.carInputParameters.neuralNetworkFile) {
			carDatas.push_back(createCarData(realTimeParameters.commonParameters, track));
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
				checkForCollisions(carData);
			}
			updateTrace();
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

void RealTimeGameManager::checkForCollisions(CarData& carData) {
	auto& model = carData.gameManager.getModel();
	auto& car = model.getCar();
	auto& track = model.getTrack();
	carData.isOut = model.hasCarCollided() || !track->isInsideTrack(car.getPosition());
	if (carData.isOut) {
		carData.outTime += physicsTimeStep;
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
	auto trackDimensions = gameManager.getTrack()->getDimensions();
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
			case sf::Keyboard::N:
				showCenterLine = !showCenterLine;
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
			case sf::Keyboard::G:
				showTrackArea = !showTrackArea;
				break;
			case sf::Keyboard::E:
				showTrace = !showTrace;
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

void RealTimeGameManager::updateTrace() {
	for (auto& carData: carDatas) {
		const auto& gameManager = carData.gameManager;
		const auto& model = gameManager.getModel();
		const Car& car = model.getCar();

		if (carData.trace.empty() || getDistanceSQ(car.getPosition(), carData.trace.back()) >
				realTimeParameters.traceOutputInterval * realTimeParameters.traceOutputInterval) {
			carData.trace.push_back(car.getPosition());
		}
	}
}

void RealTimeGameManager::drawGame() {

	if (showTrackBoundary) {
		drawTrackBoundary();
	}

	if (showCenterLine) {
		drawTrackCenterLine();
	}

	if (showCheckPoints) {
		drawTrackCheckpoints();
	}

	if (showRays) {
		drawRays();
	}

	if (showCar) {
		for (std::size_t i = 0; i < carDatas.size(); ++i) {
			drawCar(carDatas[i], i == currentCarId);
		}
	}

	if (showTrackArea) {
		drawTrackArea();
	}

	if (showTrace) {
		drawTrace();
	}

}

void RealTimeGameManager::drawTrackBoundary() {
	for (const Line2f& trackLine : carDatas[currentCarId].gameManager.getModel().getTrack()->getLines()) {
		drawLine(window, trackLine, trackEdgeColor);
	}
}

void RealTimeGameManager::drawTrackCheckpoints() {
	auto& gameManager = carDatas[currentCarId].gameManager;
	auto& model = gameManager.getModel();
	auto& track = model.getTrack();
	auto& checkpoints = track->getCheckpoints();

	for (std::size_t i = 0; i < checkpoints.size(); ++i) {

		sf::Color color = checkpointColor;
		if (model.getCurrentCheckpoint() >= 0) {
			unsigned checkpointLookAhead = gameManager.getCommonParameters().checkpointLookAhead;
			unsigned currentCheckpoint = model.getCurrentCheckpoint();
			if (i == currentCheckpoint) {
				color = checkpointNextColor;
			} else if (checkpointLookAhead > 1) {
				unsigned firstToColor = (currentCheckpoint + 1) % checkpoints.size();
				unsigned lastToColor = (currentCheckpoint + checkpointLookAhead - 1) % checkpoints.size();
				if ((firstToColor <= lastToColor) ?
						(i >= firstToColor && i <= lastToColor) :
						(i >= firstToColor || i <= lastToColor)) {
					color = checkpointLookaheadColor;
				}
			}
		}
		drawLine(window, checkpoints[i].line, color);
		auto centerPoint = (checkpoints[i].line.start + checkpoints[i].line.end) / 2.f;
		drawLine(window, centerPoint,
				centerPoint + sf::Vector2f(cos(checkpoints[i].angle), sin(checkpoints[i].angle)),
				color);
	}
}

void RealTimeGameManager::drawTrackCenterLine() {
	for (const Line2f& centerLine : carDatas[currentCarId].gameManager.getModel().getTrack()->getCenterLines()) {
		drawLine(window, centerLine, trackCenterLineColor);
	}

}

void RealTimeGameManager::drawTrace() {
	const auto& carData = carDatas[currentCarId];
	for (std::size_t i = 1; i < carData.trace.size(); ++i) {
		drawLine(window, carData.trace[i-1], carData.trace[i], traceColor);
	}

	drawLine(window, carData.trace.back(), carData.gameManager.getModel().getCar().getPosition(), traceColor);
}

void RealTimeGameManager::drawTrackArea() {
	auto& gameManager = carDatas[currentCarId].gameManager;
	auto& model = gameManager.getModel();
	auto& track = model.getTrack();

	auto sizeHalf = window.getView().getSize() / 2.f;
	auto center = window.getView().getCenter();
	sf::Vector2f min = center - sizeHalf;
	sf::Vector2f max = center + sizeHalf;
	sf::Vector2f pointSize1{areaGridPointSize, areaGridPointSize};
	sf::Vector2f pointSize2{areaGridPointSize, -areaGridPointSize};
	sf::Vector2f p;
	for (p.y = min.y; p.y < max.y; p.y += areaGridDistance) {
		for (p.x = min.x; p.x < max.x; p.x += areaGridDistance) {
			sf::Color color = track->isInsideTrack(p) ? sf::Color::Green : sf::Color::Red;
			drawLine(window, p - pointSize1, p + pointSize1, color);
			drawLine(window, p - pointSize2, p + pointSize2, color);
		}
	}
}

void RealTimeGameManager::drawCar(CarData& carData, bool isActive) {
	auto& model = carData.gameManager.getModel();
	auto& car = model.getCar();

	if (carData.isOut) {
		car.setColor(carOutColor);
	} else {
		float ratio = carData.outTime / realTimeParameters.carOutTimeout;
		sf::Color baseColor = isActive ? carActiveColor : carNormalColor;
		sf::Color color = averageColor(baseColor, carOutTimeColor, ratio);
		car.setColor(color);
	}

	car.draw(window);
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

		//auto checkpointDirection = model.getCheckpointDirection();

		std::stringstream ss;
		ss << std::fixed <<
			"FPS = " << std::setw(4) << std::setfill('0') << static_cast<int>(fps) <<
			", Speed = " << car.getSpeed() <<
			", Acceleration = " << getLength(car.getAcceleration()) <<
			", Throttle = " << car.getThrottle() <<
			",\nBrake = " << car.getBrake() <<
			//", Checkpoint = (" << checkpointDirection.x << ", " << checkpointDirection.y << ")" <<
			", TravelDistance = " << car.getTravelDistance() <<
			",\nppm = " << pixelsPerMeter;
		if (!carData.name.empty()) {
			ss <<
				", AI = " << carData.name <<
				", out = " << carData.outTime << " s";
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

