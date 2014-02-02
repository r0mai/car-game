#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Car.hpp"
#include "Telemetry.hpp"
#include "Track.hpp"

namespace car {

class GameManager {
public:
	GameManager();

	void run();

private:
	void handleInput(float deltaSeconds);
	void drawTelemetry();
	void updateTelemetry();
	void collideCar();

	sf::RenderWindow window;
	sf::View gameView;
	sf::View hudView;
	sf::Font font;

	float currentTime = 0.f;
	float fps = -1.f;

	Car car;

	bool showTelemetry = false;
	Telemetry speedTelemetry;
	Telemetry accelerationTelemetry;
	Telemetry gasTelemetry;
	Telemetry brakeTelemetry;

	Track track;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

};

}

#endif /* !GAMEMANAGER_HPP */
