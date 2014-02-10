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
	void updateTelemetry();

	Model model;
	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, 4, 10);

	bool isAIControl = false;

	sf::RenderWindow window;
	sf::View gameView;
	sf::View hudView;
	sf::Font font;

	float physicsTimeStep = 1.f/64.f;
	float fpsLimit = 64;
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
