
#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <string>

#include <boost/optional.hpp>

namespace car {

enum class PanMode {
	enabled, disabled, automatic
};

struct Parameters {

	std::string projectRootPath;

	//running mode
	bool isTrainingAI = false;

	boost::optional<unsigned> generationLimit;

	unsigned physicsTimeStepsPerSecond = 64;

	//Neural network parameters
	unsigned populationSize = 60;
	unsigned hiddenLayerCount = 2;
	unsigned neuronPerHiddenLayer = 16;

	unsigned rayCount = 14;
	//1 for speed input and 2 for the direction of the next CP
	const unsigned extraInputNeuronCount = 3;
	const unsigned outputNeuronCount = 3;

	unsigned getInputNeuronCount() const { return rayCount + extraInputNeuronCount; }

	boost::optional<std::string> neuralNetworkFile;

	//number of threads used for training
	unsigned threadCount = 1;

	//place to save the file for the best AI trained
	std::string bestAIFile = "best.car";

	//place to save/load the current population
	boost::optional<std::string> populationOutputFile;
	boost::optional<std::string> populationInputFile;

	std::vector<std::string> tracks{"circle"};

	//Parameters for random track generation
	float minRandomTrackWidth = 5.f;
	float maxRandomTrackWidth = 12.f;
	int randomTrackPoints = 10;

	unsigned startingPopulations = 1;
	unsigned populationCutoff = 10;

	//parameters for rendering
	int fpsLimit = -1;
	unsigned screenWidth = 800;
	unsigned screenHeight = 800;
	PanMode panMode = PanMode::disabled;
};

//May throw if something goes wrong with parsing
//Also may call std::exit if --help is specified
Parameters parseParameters(int argc, char **argv);

}

#endif
