#include "Benchmark.hpp"
#include <stdexcept>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include "AIGameManager.hpp"
#include <iostream>

namespace car {

Benchmark::Benchmark(const Parameters& parameters, track::TrackCreators trackCreators):
		parameters(parameters), trackCreators(trackCreators) {

	if (!parameters.neuralNetworkFile) {
		throw std::logic_error{"Neural network file is mandatory for benchmark"};
	}

	if (!parameters.generationLimit) {
		throw std::logic_error{"Generation limit is mandatory for benchmark"};
	}

	std::ifstream ifs(*parameters.neuralNetworkFile);
	boost::archive::text_iarchive ia(ifs);
	ia >> neuralNetwork;
}

void Benchmark::run() {
	std::vector<AIGameManager> managers;
	managers.reserve(trackCreators.size());
	for (const auto& trackCreator: trackCreators) {
		managers.emplace_back(parameters, trackCreator);
	}

	for (unsigned generation = 0; generation < *parameters.generationLimit; ++generation) {
		for (auto& manager: managers) {
			manager.setNeuralNetwork(neuralNetwork);
			manager.init();
			manager.run();
		}

		std::cout << "Fitnesses: ";
		for (auto& manager: managers) {
			std::cout << manager.getFitness() << " ";
		}
		std::cout << std::endl;
	}
}



}

