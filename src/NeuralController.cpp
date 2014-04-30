#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/range/algorithm.hpp>

#include "NeuralController.hpp"
#include "PopulationRunner.hpp"

namespace car {

NeuralController::NeuralController(const Parameters& parameters,
		std::vector<std::function<Track()>> trackCreators,
		boost::asio::io_service& ioService) :
	ioService(ioService),
	parameters(parameters),
	trackCreators(trackCreators)
{}

namespace {

bool compareBestFitnesses(const PopulationRunner& lhs, const PopulationRunner& rhs) {
	return lhs.getBestFitness() < rhs.getBestFitness();
}

}

void NeuralController::run() {

	std::vector<PopulationRunner> populations;
	populations.reserve(parameters.startingPopulations);

	for (std::size_t i = 0; i < parameters.startingPopulations; ++i) {
		populations.emplace_back(parameters, trackCreators, ioService);
		loadPopulation(populations.back().getPopulation());
	}

	float bestFitness = 0.f;

	for (unsigned generation = 1; !parameters.generationLimit || generation <= *parameters.generationLimit;
			++generation) {
		std::cout << "Generation: " << generation << std::endl;

		for (auto& populationData: populations) {
			populationData.runIteration();
		}

		auto& bestPopulation = *boost::max_element(populations, compareBestFitnesses);
		savePopulation(bestPopulation.getPopulation());
		if (bestPopulation.getBestFitness() > bestFitness) {
			bestFitness = bestPopulation.getBestFitness();
			std::cout << "New best fitness = " << bestFitness << std::endl;
			assert(bestPopulation.getBestGenome() != nullptr);
			saveNeuralNetwork(*bestPopulation.getBestGenome());
		}

		if (populations.size() > 1 && generation % parameters.populationCutoff == 0) {
			auto worstPopulation = boost::min_element(populations, compareBestFitnesses);
			populations.erase(worstPopulation);
		}
	}
}

void NeuralController::saveNeuralNetwork(const Genome& genome) {
	//TODO we are reconstucting the same network as above
	NeuralNetwork network(parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
			parameters.getInputNeuronCount(), parameters.outputNeuronCount);

	network.setWeights(genome.weights);

	std::ofstream ofs(parameters.bestAIFile);
	boost::archive::text_oarchive oa(ofs);
	oa << network;
}

void NeuralController::loadPopulation(GeneticPopulation& population) const {
	if (parameters.populationInputFile) {
		std::ifstream ifs(*parameters.populationInputFile);
		boost::archive::text_iarchive ia(ifs);
		ia >> population.getPopulation();
	}
}

void NeuralController::savePopulation(const GeneticPopulation& population) const {
	if (parameters.populationOutputFile) {
		std::ofstream ofs(*parameters.populationOutputFile);
		boost::archive::text_oarchive oa(ofs);
		oa << population.getPopulation();
	}
}


}

