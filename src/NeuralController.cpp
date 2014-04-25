
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <boost/archive/text_oarchive.hpp>

#include "NeuralController.hpp"
#include "AIGameManager.hpp"
#include "AsyncHelper.hpp"

namespace car {

NeuralController::NeuralController(const Parameters& parameters, std::function<Track()> trackCreator,
		boost::asio::io_service& ioService) :
	ioService(ioService),
	parameters(parameters),
	trackCreator(trackCreator)
{}

void NeuralController::run() {

	float bestFitness = 0.f;

	for (unsigned i = 0; !parameters.generationLimit || i < *parameters.generationLimit; ++i) {
		std::cout << "Generation: " << i << std::endl;

		Genomes& genomes = population.getPopulation();

		std::promise<void> genomePromise;
		std::mutex mutex;
		std::size_t tasksLeft{genomes.size()};

		for (Genome& genome : genomes) {
			ioService.post([this, &genome, &tasksLeft, &genomePromise, &mutex]() {
					NeuralNetwork network(
						parameters.hiddenLayerCount,
						parameters.neuronPerHiddenLayer,
						inputNeuronCount,
						outputNeuronCount);
					network.setWeights(genome.weights);

					AIGameManager manager(parameters, trackCreator);

					manager.setNeuralNetwork(network);
					manager.run();

					genome.fitness = manager.getFitness();

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

		float fitnessSum = 0.f;
		for (Genome& genome : genomes) {
			fitnessSum += genome.fitness;
			if (genome.fitness > bestFitness) {

				bestFitness = genome.fitness;
				std::cout << "New best fitness = " << bestFitness << std::endl;

				//TODO we are reconstucting the same network as above
				NeuralNetwork network(parameters.hiddenLayerCount, parameters.neuronPerHiddenLayer,
						inputNeuronCount, outputNeuronCount);

				network.setWeights(genome.weights);

				std::ofstream ofs(parameters.bestAIFile);
				boost::archive::text_oarchive oa(ofs);
				oa << network;
			}
		}
		std::cout << "Population average = " << fitnessSum / population.getPopulation().size() << std::endl;
		population.evolve();
	}
}

}

