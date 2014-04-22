
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

	unsigned populationSize = 30;

	unsigned hiddenLayerCount = 1;
	unsigned neuronPerHidden = 9;
	unsigned inputNeuronCount = 8;
	unsigned outputNeuronCount = 3;

	GeneticPopulation population = GeneticPopulation(
			populationSize,
			NeuralNetwork::getWeightCountForNetwork(
				hiddenLayerCount, neuronPerHidden,
				inputNeuronCount, outputNeuronCount));
};

}

#endif

