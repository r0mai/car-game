
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <vector>
#include <functional>

#include "Model.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class GameManager {
public:

	GameManager(std::function<Track()> trackCreator);

	void advance();

	void setNeuralNetwork(const NeuralNetwork& network);

protected:
	void handleInput();
	virtual void handleUserInput();

	Weights callNeuralNetwork();

	float physicsTimeStep = 1.f/64.f;

	const unsigned rayCount = 7;
	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	//+1 is for speed input
	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, rayCount + 1, 3);

	bool isAIControl = true;

	Model model;
};

}

#endif
