#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"
#include "Track/TrackCreator.hpp"
#include "IterationParameters.hpp"

namespace car {

class AIGameManager {
public:
	AIGameManager(const CommonParameters& commonParameters,
			const IterationParameters& iterationParameters, track::TrackCreator trackCreator);

	void run();

	void setNeuralNetwork(const NeuralNetwork& neuralNetwork) {
		gameManager.setNeuralNetwork(neuralNetwork);
	}

	void init();
	const GameManager& getGameManager() const { return gameManager; }

private:
	bool stopCondition() const;

	GameManager gameManager;
	IterationParameters iterationParameters;
};

}

#endif
