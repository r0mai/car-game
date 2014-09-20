#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "NeuralNetwork.hpp"
#include "BenchmarkParameters.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class Benchmark {

public:

	Benchmark(const BenchmarkParameters& parameters,
			std::vector<std::shared_ptr<const track::Track>> tracks);

	void run();

private:

	BenchmarkParameters parameters;
	std::vector<std::shared_ptr<const track::Track>> tracks;
	std::vector<NeuralNetwork> neuralNetworks;

};

}


#endif /* BENCHMARK_HPP */
