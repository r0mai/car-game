
#include <iostream>
#include <fstream>
#include <future>
#include <thread>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "NeuralController.hpp"
#include "AIGameManager.hpp"
#include "AsyncHelper.hpp"

namespace car {

NeuralController::NeuralController(const Parameters& parameters,
		std::vector<std::function<Track()>> trackCreators,
		boost::asio::io_service& ioService) :
	ioService(ioService),
	parameters(parameters),
	trackCreators(trackCreators)
{}

struct NeuralControllerData {
	NeuralNetwork network;
	std::vector<AIGameManager> managers;
};

void NeuralController::run() {

	loadPopulation();

	float bestFitness = 0.f;

	std::vector<NeuralControllerData> datas;
	datas.reserve(parameters.populationSize);
	for (std::size_t i = 0; i < parameters.populationSize; ++i) {
		datas.push_back(NeuralControllerData{
			{
				parameters.hiddenLayerCount,
				parameters.neuronPerHiddenLayer,
				parameters.getInputNeuronCount(),
				parameters.outputNeuronCount
			},
			{}
		});

		datas.back().managers.reserve(trackCreators.size());
		for (const auto& trackCreator: trackCreators) {
			datas.back().managers.emplace_back(parameters, trackCreator);
		}
	}

	for (unsigned i = 0; !parameters.generationLimit || i < *parameters.generationLimit; ++i) {
		std::cout << "Generation: " << i << std::endl;

		savePopulation();

		Genomes& genomes = population.getPopulation();
		assert(genomes.size() == parameters.populationSize);

		std::promise<void> genomePromise;
		std::mutex mutex;
		std::size_t tasksLeft{genomes.size()};

		for (std::size_t i = 0; i < datas.size(); ++i) {
			auto& genome = genomes[i];
			auto& data = datas[i];
			ioService.post([this, &genome, &data, &tasksLeft, &genomePromise, &mutex]() {
					runSimulation(genome, data);

					int value;
					{
						std::unique_lock<std::mutex> lock{mutex};
						value = --tasksLeft;
					}
					if (value == 0) {
						genomePromise.set_value();
					}
				});
		}

		auto genomeFuture = genomePromise.get_future();
		genomeFuture.wait();

		updateBestFitness(genomes, bestFitness);
		population.evolve();
	}
}

void NeuralController::runSimulation(Genome& genome, NeuralControllerData& data) {
	data.network.setWeights(genome.weights);
	genome.fitness = 0;

	for (auto& manager: data.managers) {
		manager.setNeuralNetwork(data.network);
		manager.init();
		manager.run();
		genome.fitness += manager.getFitness();
	}
}

void NeuralController::updateBestFitness(Genomes& genomes, float& bestFitness) {
	float fitnessSum = 0.f;
	for (Genome& genome : genomes) {
		fitnessSum += genome.fitness;
		if (genome.fitness > bestFitness) {

			bestFitness = genome.fitness;
			std::cout << "New best fitness = " << bestFitness << std::endl;

			//TODO we are reconstucting the same network as above
			NeuralNetwork network(parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
					parameters.getInputNeuronCount(), parameters.outputNeuronCount);

			network.setWeights(genome.weights);

			std::ofstream ofs(parameters.bestAIFile);
			boost::archive::text_oarchive oa(ofs);
			oa << network;
		}
	}
	std::cout << "Population average = " << fitnessSum / population.getPopulation().size() << std::endl;
}

void NeuralController::loadPopulation() {
	if (parameters.populationInputFile) {
		std::ifstream ifs(*parameters.populationInputFile);
		boost::archive::text_iarchive ia(ifs);
		ia >> population.getPopulation();
	}
}

void NeuralController::savePopulation() const {
	if (parameters.populationOutputFile) {
		std::ofstream ofs(*parameters.populationOutputFile);
		boost::archive::text_oarchive oa(ofs);
		oa << population.getPopulation();
	}
}

}

