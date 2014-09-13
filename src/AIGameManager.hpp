#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class AIGameManager {
public:
	AIGameManager(const CommonParameters& parameters, track::TrackCreator trackCreator);

	void run();

	void setNeuralNetwork(const NeuralNetwork& neuralNetwork) {
		gameManager.setNeuralNetwork(neuralNetwork);
	}

	void init();
	const GameManager& getGameManager() const { return gameManager; }

private:
	bool stopCondition() const;

	GameManager gameManager;
	const float maxTime = 600.f;
};

}

#endif
