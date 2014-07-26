
#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <string>
#include "RealTimeParameters.hpp"
#include "LearningParameters.hpp"
#include "BenchmarkParameters.hpp"

namespace car {

enum class GameType {
	realtime, learning, benchmark
};

struct Parameters {

	//running mode
	GameType gameType = GameType::realtime;
	std::vector<std::string> tracks;

	RealTimeParameters realTimeParameters;
	LearningParameters learningParameters;
	BenchmarkParameters benchmarkParameters;
};

//May throw if something goes wrong with parsing
//Also may call std::exit if --help is specified
Parameters parseParameters(int argc, char **argv);

}

#endif
