
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

	GeneticPopulation population = GeneticPopulation(
			parameters.populationSize,
			NeuralNetwork::getWeightCountForNetwork(
				parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
				parameters.getInputNeuronCount(), parameters.outputNeuronCount));

	void runSimulation(Genome& genome, NeuralControllerData& data);
	void updateBestFitness(Genomes& genomes, float& bestFitness);
	void runIteration(Genomes& genomes, std::vector<NeuralControllerData>& datas);
};

}

#endif

