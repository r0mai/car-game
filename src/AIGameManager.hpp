#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "Model.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class AIGameManager {
public:
	AIGameManager();

	void run();
private:
	float physicsTimeStep = 1.f/64.f;

	Model model;
	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, 4, 10);
};

}

#endif
