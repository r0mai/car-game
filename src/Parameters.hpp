
#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <string>

#include <boost/optional.hpp>

#include "TrackType.hpp"

namespace car {

struct Parameters {
	//running mode
	bool isTrainingAI = false;

	//Neural network parameters
	unsigned populationSize = 60;
	unsigned hiddenLayerCount = 2;
	unsigned neuronPerHiddenLayer = 16;

	boost::optional<std::string> neuralNetworkFile;

	//track type
	TrackType trackType = TrackType::circle;

	//Parameters for random track generation
	float minRandomTrackWidth = 5.f;
	float maxRandomTrackWidth = 12.f;
	unsigned randomTrackSeed = 0;
	int randomTrackPoints = 10;

	//parameters for rendering
	int fpsLimit = -1;

};

//May throw if something goes wrong with parsing
//Also may call std::exit if --help is specified
Parameters parseParameters(int argc, char **argv);

}

#endif
