
#ifndef NEURALCONTROLLER_HPP_
#define NEURALCONTROLLER_HPP_

#include <functional>

#include "GeneticPopulation.hpp"
#include "NeuralNetwork.hpp"
#include "Parameters.hpp"
#include "Track.hpp"
#include "boost/asio/io_service.hpp"

namespace car {

struct NeuralControllerData;

class NeuralController {
public:
	NeuralController(const Parameters& parameters,
			std::vector<std::function<Track()>> trackCreators,
			boost::asio::io_service& ioService);
	void run();

private:
	void loadPopulation();
	void savePopulation() const;

	boost::asio::io_service& ioService;
	Parameters parameters;
	std::vector<std::function<Track()>> trackCreators;

	unsigned inputNeuronCount = 15;
	unsigned outputNeuronCount = 3;

	GeneticPopulation population = GeneticPopulation(
			parameters.populationSize,
			NeuralNetwork::getWeightCountForNetwork(
				parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
				inputNeuronCount, outputNeuronCount));

	void runSimulation(Genome& genome, NeuralControllerData& data);
	void updateBestFitness(Genomes& genomes, float& bestFitness);
};

}

#endif

