#ifndef REALTIMEGAMEMANAGER_HPP
#define REALTIMEGAMEMANAGER_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "Model.hpp"
#include "Telemetry.hpp"

namespace car {

class RealTimeGameManager {
public:
	RealTimeGameManager();

	void run();

private:
	void handleInput();
	void drawTelemetry();
	void updateTelemetry();

	Model model;

	sf::RenderWindow window;
	sf::View gameView;
	sf::View hudView;
	sf::Font font;

	float fps = -1.f;

	bool showTelemetry = false;
	Telemetry speedTelemetry;
	Telemetry accelerationTelemetry;
	Telemetry gasTelemetry;
	Telemetry brakeTelemetry;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

};

}

#endif /* !REALTIMEGAMEMANAGER_HPP */
