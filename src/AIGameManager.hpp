#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"
#include "Track/TrackCreator.hpp"
#include "MathExpression.hpp"

namespace car {

class AIGameManager {
public:
	AIGameManager(const CommonParameters& parameters, track::TrackCreator trackCreator,
			const MathExpression& fitnessExpression);

	void run();

	//should be called after run()
	float getFitness() const;

	void setNeuralNetwork(const NeuralNetwork& neuralNetwork) {
		gameManager.setNeuralNetwork(neuralNetwork);
	}

	void init();

private:
	bool stopCondition() const;

	GameManager gameManager;
	const float maxTime = 600.f;
	MathExpression fitnessExpression;
};

}

#endif
