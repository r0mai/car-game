#include "Benchmark.hpp"
#include <stdexcept>
#include <iostream>
#include "AIGameManager.hpp"
#include "Line2.hpp"

namespace car {

Benchmark::Benchmark(const BenchmarkParameters& parameters, track::TrackCreators trackCreators):
		parameters(parameters), trackCreators(trackCreators) {

	if (parameters.carInputParameters.neuralNetworkFile.empty()) {
		throw std::logic_error{"Neural network file is mandatory for benchmark"};
	}

	if (!parameters.iterationParameters.generationLimit) {
		throw std::logic_error{"Generation limit is mandatory for benchmark"};
	}

	for (const auto& filename: parameters.carInputParameters.neuralNetworkFile) {
		neuralNetworks.push_back(loadNeuralNetworkFromFile(filename));
	}

}

void Benchmark::run() {
	std::vector<AIGameManager> managers;
	managers.reserve(trackCreators.size());
	for (const auto& trackCreator: trackCreators) {
		managers.emplace_back(parameters.commonParameters, trackCreator,
				parameters.iterationParameters.fitnessExpression);
	}

	for (unsigned generation = 1;
			generation <= *parameters.iterationParameters.generationLimit;
			++generation) {
		for (const auto& neuralNetwork: neuralNetworks) {
			for (auto& manager: managers) {
				manager.setNeuralNetwork(neuralNetwork);
				manager.init();
				manager.run();
			}

			if (generation % parameters.iterationParameters.printoutFrequency == 0) {
				std::cout << "Fitnesses: ";
				for (auto& manager: managers) {
					std::cout << manager.getFitness() << " ";
				}
				std::cout << std::endl;
			}
		}
	}
}



}

