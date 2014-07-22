
#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <string>
#include <thread>

#include <boost/optional.hpp>

#include "MathExpression.hpp"
#include "ScreenDimension.hpp"

namespace car {

enum class PanMode {
	center, fit
};

enum class GameType {
	realtime, learning, benchmark
};

struct Parameters {

	std::string projectRootPath;

	//running mode
	GameType gameType = GameType::realtime;

	boost::optional<unsigned> generationLimit;
	unsigned printoutFrequency = 1;

	unsigned physicsTimeStepsPerSecond = 64;

	//Neural network parameters
	unsigned populationSize = 60;
	unsigned hiddenLayerCount = 2;
	unsigned neuronPerHiddenLayer = 16;
	bool useRecurrence = false;

	unsigned rayCount = 14;
	//1 for speed input and 2 for the direction of the next CP
	const unsigned extraInputNeuronCount = 3;
	const unsigned outputNeuronCount = 3;

	unsigned getInputNeuronCount() const { return rayCount + extraInputNeuronCount; }

	boost::optional<std::string> neuralNetworkFile;

	//number of threads used for training
	unsigned threadCount = [] { auto j = std::thread::hardware_concurrency(); return j > 0 ? j : 1; }();

	//place to save the file for the best AI trained
	std::string bestAIFile = "best.car";

	//place to save/load the current population
	boost::optional<std::string> populationOutputFile;
	boost::optional<std::string> populationInputFile;

	std::vector<std::string> tracks;

	unsigned startingPopulations = 1;
	unsigned populationCutoff = 10;

	MathExpression fitnessExpression = parseMathExpression(
		"0.5*td + (ccps > cps)*(100*cps + 2*(ccps-cps)) + (ccps <= cps)*(100*ccps)");

	//parameters for rendering
	int fpsLimit = -1;
	unsigned screenWidth = 800;
	unsigned screenHeight = 800;
	float minPixelsPerMeter = 5.f;
	float maxPixelsPerMeter = 10.f;
	PanMode panMode = PanMode::fit;
	ScreenDimension panThreshold{Meters(0)};
};

//May throw if something goes wrong with parsing
//Also may call std::exit if --help is specified
Parameters parseParameters(int argc, char **argv);

}

#endif
