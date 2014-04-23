
#ifndef NEURALCONTROLLER_HPP_
#define NEURALCONTROLLER_HPP_

#include <functional>

#include "GeneticPopulation.hpp"
#include "NeuralNetwork.hpp"
#include "Track.hpp"

namespace car {

class NeuralController {
public:
	NeuralController(std::function<Track()> trackCreator);
	void run();
private:
	std::function<Track()> trackCreator;

	unsigned populationSize = 60;

	unsigned hiddenLayerCount = 2;
	unsigned neuronPerHidden = 16;
	unsigned inputNeuronCount = 14;
	unsigned outputNeuronCount = 3;

	GeneticPopulation population = GeneticPopulation(
			populationSize,
			NeuralNetwork::getWeightCountForNetwork(
				hiddenLayerCount, neuronPerHidden,
				inputNeuronCount, outputNeuronCount));
};

}

#endif

