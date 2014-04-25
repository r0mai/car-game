#ifndef REALTIMEGAMEMANAGER_HPP
#define REALTIMEGAMEMANAGER_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "GameManager.hpp"
#include "Telemetry.hpp"

namespace car {

class Track;

class RealTimeGameManager : public GameManager {
public:
	RealTimeGameManager(const Parameters& parameter, std::function<Track()> trackCreator);

	void run();

	void setFPSLimit(float newFPSLimit);

protected:
	virtual void handleUserInput();

	void drawTelemetry();
	void drawRayPoints();
	void updateTelemetry();

	sf::RenderWindow window;
	sf::View gameView;
	sf::View hudView;
	sf::Font font;

	float fpsLimit = 64;
	float fps = -1.f;

	bool showTelemetry = false;
	Telemetry speedTelemetry;
	Telemetry accelerationTelemetry;
	Telemetry angleTelemetry;
	Telemetry gasTelemetry;
	Telemetry brakeTelemetry;
	Telemetry turnTelemetry;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

};

}

#endif /* !REALTIMEGAMEMANAGER_HPP */
