#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Car.hpp"
#include "Telemetry.hpp"

namespace car {

class GameManager {
public:
	GameManager();

	void run();

private:
	void handleInput(float deltaSeconds);
	void drawCar();
	void drawTelemetry();
	void updateTelemetry();

	sf::RenderWindow window;
	sf::Font font;

	float currentTime = 0.f;

	Car car;
	Telemetry speedTelemetry;
	Telemetry accelerationTelemetry;
	Telemetry gasTelemetry;
	Telemetry brakeTelemetry;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

};

}

#endif /* !GAMEMANAGER_HPP */
