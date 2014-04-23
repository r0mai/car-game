
#include <iostream>
#include <fstream>
#include <future>

#include <boost/archive/text_oarchive.hpp>

#include "NeuralController.hpp"
#include "AIGameManager.hpp"

namespace car {

NeuralController::NeuralController(std::function<Track()> trackCreator) :
   trackCreator(trackCreator) {}

void NeuralController::run() {

	float bestFitness = 0.f;

	for (unsigned i = 0; ; ++i) {
		std::cout << "Generation: " << i << std::endl;

		Genomes& genomes = population.getPopulation();

		std::vector<std::future<void>> genomeFutures;
		genomeFutures.reserve(genomes.size());

		for (Genome& genome : genomes) {
			genomeFutures.emplace_back(
				std::async(std::launch::async,
					[this, &genome]() {
						NeuralNetwork network(hiddenLayerCount, neuronPerHidden,
								inputNeuronCount, outputNeuronCount);

						network.setWeights(genome.weights);

						AIGameManager manager(trackCreator);

						manager.setNeuralNetwork(network);
						manager.run();

						genome.fitness = manager.getFitness();

					})
				);
		}

		for (auto& future : genomeFutures) {
			future.wait();
		}

		float fitnessSum = 0.f;
		for (Genome& genome : genomes) {
			fitnessSum += genome.fitness;
			if (genome.fitness > bestFitness) {

				bestFitness = genome.fitness;
				std::cout << "New best fitness = " << bestFitness << std::endl;

				//TODO we are reconstucting the same network as above
				NeuralNetwork network(hiddenLayerCount, neuronPerHidden,
						inputNeuronCount, outputNeuronCount);

				network.setWeights(genome.weights);

				std::ofstream ofs("best.car");
				boost::archive::text_oarchive oa(ofs);
				oa << network;
			}
		}
		std::cout << "Population average = " << fitnessSum / population.getPopulation().size() << std::endl;

		population.evolve();
	}
}

}

