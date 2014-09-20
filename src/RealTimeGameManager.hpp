#ifndef REALTIMEGAMEMANAGER_HPP
#define REALTIMEGAMEMANAGER_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "GameManager.hpp"
#include "Telemetry.hpp"
#include "Track/TrackCreator.hpp"
#include "RealTimeParameters.hpp"

namespace car {

namespace track {
	class Track;
}

class RealTimeGameManager {
public:
	RealTimeGameManager(const RealTimeParameters& realTimeParameters,
			std::shared_ptr<const track::Track> track);

	void run();

	void setFPSLimit(float newFPSLimit);

private:
	struct CarData {
		GameManager gameManager;
		Telemetry speedTelemetry;
		Telemetry accelerationTelemetry;
		Telemetry angleTelemetry;
		Telemetry gasTelemetry;
		Telemetry brakeTelemetry;
		Telemetry turnTelemetry;
		std::vector<sf::Vector2f> trace;
		std::string name;
		bool isOut = false;
		float outTime = 0.0;

		CarData(const CommonParameters& parameters, std::shared_ptr<const track::Track> track):
			gameManager{parameters, std::move(track)} {}
	};

	void handleUserInput();

	void drawGame();
	void drawTelemetry();
	void drawRays();
	void updateTelemetry();
	void updateTrace();
	void drawCar(CarData& carData, bool isActive);
	void drawTrackArea();
	void drawTrace();

	std::vector<CarData> carDatas;
	std::size_t currentCarId = 0;

	sf::RenderWindow window;
	sf::View gameView;
	sf::View hudView;
	sf::Font font;

	float fpsLimit = 64;
	float fps = -1.f;

	bool showCar = true;
	bool showRays = true;
	bool showCheckPoints = true;
	bool showTrackBoundary = true;
	bool showTelemetryText = true;
	bool showTelemetryGraphs = false;
	bool showTrackArea = false;
	bool showTrace = false;
	float pixelsPerMeter = 0.f;
	float panThreshold = 0.f;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

	float calculateCenter(float viewSize, float trackOrigin, float trackSize, float carPosition);
	void setViewParameters();
	void checkForCollisions(CarData& carData);

	RealTimeParameters realTimeParameters;
	float physicsTimeStep;

	static CarData createCarData(const CommonParameters& parameters, 
			std::shared_ptr<const track::Track> track);

	static const float areaGridDistance;
	static const float areaGridPointSize;
	static const sf::Color carNormalColor;
	static const sf::Color carActiveColor;
	static const sf::Color carOutColor;
	static const sf::Color carOutTimeColor;
	static const float carOutTimeout;
	static const sf::Color traceColor;
	static const sf::Color rayColor;
};

}

#endif /* !REALTIMEGAMEMANAGER_HPP */
