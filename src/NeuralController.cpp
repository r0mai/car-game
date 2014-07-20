
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/range/algorithm.hpp>

#include "NeuralController.hpp"
#include "PopulationRunner.hpp"

namespace car {

NeuralController::NeuralController(const Parameters& parameters,
		track::TrackCreators trackCreators,
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

static void printInfo(unsigned generation, float bestFitness, const std::vector<float>& populationAverages) {
	std::stringstream ss;
	ss << "Generation: " << generation << ", ";
	ss << "Current best fitness: " << bestFitness << ", ";
	ss << "Population averages: ";
	for (float a : populationAverages) {
		 ss << a << ", ";
	}
	if (isatty(1)) { //if stdout is a terminal
		std::cout << "\033[2K\r";
		std::cout << ss.str() << std::flush;
	} else {
		std::cout << ss.str() << std::endl;
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

		std::vector<float> populationAverages;
		for (auto& populationData: populations) {
			populationData.runIteration();
			populationAverages.push_back(populationData.getAverageFitness());
		}

		auto& bestPopulation = *boost::max_element(populations, compareBestFitnesses);
		savePopulation(bestPopulation.getPopulation());
		if (bestPopulation.getBestFitness() > bestFitness) {
			bestFitness = bestPopulation.getBestFitness();
			assert(bestPopulation.getBestGenome() != nullptr);
			saveNeuralNetwork(*bestPopulation.getBestGenome());
		}

		if (populations.size() > 1 && generation % parameters.populationCutoff == 0) {
			auto worstPopulation = boost::min_element(populations, compareBestFitnesses);
			populations.erase(worstPopulation);
		}
		printInfo(generation, bestFitness, populationAverages);
	}
}

void NeuralController::saveNeuralNetwork(const Genome& genome) {
	//TODO we are reconstucting the same network as above
	NeuralNetwork network(parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
			parameters.getInputNeuronCount(), parameters.outputNeuronCount, parameters.useRecurrence);

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

