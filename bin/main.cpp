
#include "RealTimeGameManager.hpp"
#include "NeuralController.hpp"
#include "Parameters.hpp"
#include "TrackType.hpp"
#include "Track.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/program_options.hpp>

using namespace car;

int main(int argc, char **argv) {

	namespace po = boost::program_options;

	Parameters parameters;

	TrackType trackType = TrackType::circle;
	std::string configFile;

	po::options_description commandLineDescription("Command line options");
	commandLineDescription.add_options()
		("help", "produce help message")
		("ai", "simulate AI")
		("config", po::value<std::string>(), "reads configuration parameters from the specified file")
		("neural-network", po::value<std::string>(), "load neural-network from file")
		("track", po::value<TrackType>(&trackType)->default_value(TrackType::circle),
				"The type of track to use. Allowed values: circle, zigzag, curvy")
		("fps-limit", po::value<int>()->default_value(-1), "set fps limit. negative value means no limit")
	;

	po::options_description configFileDescription("Config file options");
	configFileDescription.add_options()
		("population-size", po::value<unsigned>(&parameters.populationSize), "size of the population used in the genetic algorithm")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, commandLineDescription), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << commandLineDescription << std::endl;
		return 1;
	}

	if (vm.count("config")) {
		po::store(po::parse_config_file<char>(vm["config"].as<std::string>().c_str(), configFileDescription), vm);
		po::notify(vm);
	}

	std::function<Track()> trackCreator;

	switch (trackType) {
	case TrackType::circle:
		trackCreator = createCircleTrack;
		break;
	case TrackType::zigzag:
		trackCreator = createZigZagTrack;
		break;
	case TrackType::curvy:
		trackCreator = createCurvyTrack;
		break;
	}

	std::srand(std::time(0));
	if (vm.count("ai")) {
		NeuralController controller{parameters, trackCreator};
		controller.run();
	} else {
		RealTimeGameManager manager{trackCreator};

		manager.setFPSLimit(vm["fps-limit"].as<int>());

		if (vm.count("neural-network")) {
			NeuralNetwork network;

			std::ifstream ifs(vm["neural-network"].as<std::string>());
			boost::archive::text_iarchive ia(ifs);
			ia >> network;

			manager.setNeuralNetwork(network);
		}
		manager.run();
	}
	return 0;
}

