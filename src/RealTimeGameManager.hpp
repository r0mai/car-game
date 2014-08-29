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
			track::TrackCreator trackCreator);

	void run();

	void setFPSLimit(float newFPSLimit);

private:
	void handleUserInput();

	void drawGame();
	void drawTelemetry();
	void drawRays();
	void updateTelemetry();
	void drawCar(GameManager& gameManager);

	struct CarData {
		GameManager gameManager;
		Telemetry speedTelemetry;
		Telemetry accelerationTelemetry;
		Telemetry angleTelemetry;
		Telemetry gasTelemetry;
		Telemetry brakeTelemetry;
		Telemetry turnTelemetry;
		std::string name;

		CarData(const CommonParameters& parameters, track::TrackCreator trackCreator):
			gameManager{parameters, trackCreator} {}
	};

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
	float pixelsPerMeter = 0.f;
	float panThreshold = 0.f;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

	float calculateCenter(float viewSize, float trackOrigin, float trackSize, float carPosition);
	void setViewParameters();

	RealTimeParameters realTimeParameters;
	float physicsTimeStep;

	static CarData createCarData(const CommonParameters& parameters, track::TrackCreator trackCreator);
};

}

#endif /* !REALTIMEGAMEMANAGER_HPP */
