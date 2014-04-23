
#include <iostream>
#include <fstream>

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

		float fitnessSum = 0.0;
		for (Genome& genome : population.getPopulation()) {

			NeuralNetwork network(hiddenLayerCount, neuronPerHidden,
					inputNeuronCount, outputNeuronCount);

			network.setWeights(genome.weights);

			AIGameManager manager(trackCreator);

			manager.setNeuralNetwork(network);
			manager.run();

			genome.fitness = manager.getFitness();
			fitnessSum += genome.fitness;
			if (genome.fitness > bestFitness) {

				bestFitness = genome.fitness;
				std::cout << "New best fitness = " << bestFitness << std::endl;

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

