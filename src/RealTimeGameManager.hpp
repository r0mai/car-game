#ifndef REALTIMEGAMEMANAGER_HPP
#define REALTIMEGAMEMANAGER_HPP

#include <vector>
#include <functional>

#include <SFML/Graphics.hpp>

#include "Model.hpp"
#include "Telemetry.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class Track;

class RealTimeGameManager {
public:
	RealTimeGameManager(std::function<Track()> trackCreator);

	void setFPSLimit(float newFpsLimit);

	void run();

private:
	void handleInput();
	void drawTelemetry();
	void drawRayPoints();
	void updateTelemetry();
	Weights callNeuralNetwork();

	Model model;

	const unsigned rayCount = 7;
	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	//+1 is for speed input
	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, rayCount + 1, 3);

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
	Telemetry angleTelemetry;
	Telemetry gasTelemetry;
	Telemetry brakeTelemetry;
	Telemetry turnTelemetry;

	bool pressedKeys[sf::Keyboard::KeyCount] = {false};

};

}

#endif /* !REALTIMEGAMEMANAGER_HPP */
