
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <vector>
#include <functional>

#include "Model.hpp"
#include "CommonParameters.hpp"
#include "NeuralNetwork.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class GameManager {
public:

	GameManager(const CommonParameters& parameters, track::TrackCreator trackCreator);

	void advance();

	void setNeuralNetwork(const NeuralNetwork& network);

	void init();
protected:
	void handleInput();
	virtual void handleUserInput();

	Weights callNeuralNetwork();

	CommonParameters parameters;

	float physicsTimeStep = 1.f/parameters.physicsTimeStepsPerSecond;

	// We have to store rayCount here as well, because with setNeuralNetwork it
	// is possible to set a neuralNetwork which is not consistent with what is
	// specified in parameters TODO something has to be done about who stores
	// the variables concerning the number of inputs/outputs
	unsigned rayCount = parameters.rayCount;
	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	NeuralNetwork neuralNetwork;

	bool isAIControl = true;

	Model model;
	track::Track track;
};

}

#endif

