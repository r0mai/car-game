
#ifndef NEURALCONTROLLER_HPP_
#define NEURALCONTROLLER_HPP_

#include <functional>

#include "GeneticPopulation.hpp"
#include "NeuralNetwork.hpp"
#include "Parameters.hpp"
#include "Track.hpp"

namespace car {

class NeuralController {
public:
	NeuralController(const Parameters& parameters, std::function<Track()> trackCreator);
	void run();
private:
	Parameters parameters;
	std::function<Track()> trackCreator;

	unsigned inputNeuronCount = 14;
	unsigned outputNeuronCount = 3;

	GeneticPopulation population = GeneticPopulation(
			parameters.populationSize,
			NeuralNetwork::getWeightCountForNetwork(
				parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
				inputNeuronCount, outputNeuronCount));
};

}

#endif

