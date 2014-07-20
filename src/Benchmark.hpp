#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "NeuralNetwork.hpp"
#include "Parameters.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class Benchmark {

public:

	Benchmark(const Parameters& parameters, track::TrackCreators trackCreators);

	void run();

private:

	Parameters parameters;
	track::TrackCreators trackCreators;
	NeuralNetwork neuralNetwork;

};

}


#endif /* BENCHMARK_HPP */
