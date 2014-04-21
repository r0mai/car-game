#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include <functional>
#include "Model.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class Track;

class AIGameManager {
public:
	AIGameManager(std::function<Track()> trackCreator);

	void run();
private:
	float physicsTimeStep = 1.f/64.f;

	Model model;
	NeuralNetwork neuralNetwork = NeuralNetwork(1, 10, 4, 10);
};

}

#endif
