#ifndef LEARNINGPARAMETERS_HPP
#define LEARNINGPARAMETERS_HPP

#include <thread>
#include <boost/optional.hpp>
#include "CommonParameters.hpp"
#include "IterationParameters.hpp"

namespace car {

struct LearningParameters {
	CommonParameters commonParameters;
	IterationParameters iterationParameters;

	//Neural network parameters
	unsigned hiddenLayerCount = 1;
	unsigned neuronPerHiddenLayer = 60;
	bool useRecurrence = false;

	unsigned populationSize = 80;

	//number of threads used for training
	unsigned threadCount = [] { auto j = std::thread::hardware_concurrency(); return j > 0 ? j : 1; }();

	//place to save the file for the best AI trained
	std::string bestAIFile = "best.car";

	//place to save/load the current population
	boost::optional<std::string> populationOutputFile;
	boost::optional<std::string> populationInputFile;

	unsigned startingPopulations = 1;
	unsigned populationCutoff = 10;
};

}


#endif /* LEARNINGPARAMETERS_HPP */
