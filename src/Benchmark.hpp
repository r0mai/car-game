#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "NeuralNetwork.hpp"
#include "BenchmarkParameters.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class Benchmark {

public:

	Benchmark(const BenchmarkParameters& parameters, track::TrackCreators trackCreators);

	void run();

private:

	BenchmarkParameters parameters;
	track::TrackCreators trackCreators;
	NeuralNetwork neuralNetwork;

};

}


#endif /* BENCHMARK_HPP */
