
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <vector>
#include <functional>

#include "Model.hpp"
#include "Parameters.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class GameManager {
public:

	GameManager(const Parameters& parameters, std::function<Track()> trackCreator);

	void advance();

	void setNeuralNetwork(const NeuralNetwork& network);

protected:
	void handleInput();
	virtual void handleUserInput();

	Weights callNeuralNetwork();

	Parameters parameters;

	float physicsTimeStep = 1.f/parameters.physicsTimeStepsPerSecond;

	unsigned rayCount = 13;
	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	//+2 is for speed input and for the direction of the next CP
	static constexpr int extraNeuronCount = 2;

	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, rayCount + extraNeuronCount, 3);

	bool isAIControl = true;

	Model model;
};

}

#endif

