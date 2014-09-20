#include "Benchmark.hpp"
#include <stdexcept>
#include <iostream>
#include <boost/range/adaptor/transformed.hpp>
#include "AIGameManager.hpp"
#include "Line2.hpp"
#include "lua/Lua.hpp"
#include "FitnessCalculator.hpp"

namespace car {

Benchmark::Benchmark(const BenchmarkParameters& parameters,
		std::vector<std::shared_ptr<const track::Track>> tracks):
		parameters(parameters), tracks(std::move(tracks)) {

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
	lua::Lua lua;
	lua.loadFile(parameters.iterationParameters.fitnessScript);
	FitnessCalculator fitnessCalculator(lua);

	std::vector<AIGameManager> managers;
	managers.reserve(tracks.size());
	for (const auto& track: tracks) {
		managers.emplace_back(parameters.commonParameters,
				parameters.iterationParameters,
				track);
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
				std::cout << "Fitness: ";
				auto range = managers |
					boost::adaptors::transformed([](const AIGameManager& manager) {
						return manager.getGameManager().getModel();
					});
					std::string debugInfo;
					std::cout << fitnessCalculator.calculateFitness(
							range.begin(), range.end(), &debugInfo);
					std::cout << " " << debugInfo << std::endl;
			}
		}
	}
}



}

