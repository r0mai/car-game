
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

	void init();
protected:
	void handleInput();
	virtual void handleUserInput();

	Weights callNeuralNetwork();

	Parameters parameters;

	float physicsTimeStep = 1.f/parameters.physicsTimeStepsPerSecond;

	//We have to store rayCount here as well, because with setNeuralNetwork it is possible to
	//set a neuralNetwork which is not consistent with what is specified in parameters
	//TODO something has to be done about who stores the variables concerning the number of
	//inputs/outputs
	unsigned rayCount = parameters.rayCount;
	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	NeuralNetwork neuralNetwork = NeuralNetwork(parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
		   rayCount + parameters.extraInputNeuronCount, parameters.outputNeuronCount);

	bool isAIControl = true;

	Model model;
	Track track;
};

}

#endif

