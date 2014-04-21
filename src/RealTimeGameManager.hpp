#ifndef REALTIMEGAMEMANAGER_HPP
#define REALTIMEGAMEMANAGER_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "Model.hpp"
#include "Telemetry.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class RealTimeGameManager {
public:
	RealTimeGameManager();

	void setFPSLimit(float newFpsLimit);

	void run();

private:
	void handleInput();
	void drawTelemetry();
	void drawRayPoints();
	void updateTelemetry();

	Model model;
	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, 4, 10);

	bool isAIControl = false;

	sf::RenderWindow window;
	sf::View gameView;
	sf::View hudView;
	sf::Font font;

	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	float physicsTimeStep = 1.f/64.f;
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
