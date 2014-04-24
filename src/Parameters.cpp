
#include "Parameters.hpp"

#include <boost/program_options.hpp>

namespace car {

Parameters parseParameters(int argc, char **argv) {

	namespace po = boost::program_options;

	Parameters parameters;

	std::string configFile;

	po::options_description commandLineOnlyDescription("Command-line only options");
	commandLineOnlyDescription.add_options()
		("help", "produce help message")
		("ai", "train AI")
		("config", po::value<std::string>(), "reads configuration parameters from the specified file")
	;

	po::options_description configFileDescription("Command-line and config file options");
	configFileDescription.add_options()
		("population-size", po::value<unsigned>(&parameters.populationSize),
				"size of the population used in the genetic algorithm")
		("hidden-layer-count", po::value<unsigned>(&parameters.hiddenLayerCount),
				"number of hidden layers in the nerual network")
		("neuron-per-hidden-layer", po::value<unsigned>(&parameters.neuronPerHiddenLayer),
				"number of neurons/hidden layer in the nerual network")
		("neural-network", po::value<std::string>(),
				"load neural-network from file")
		("track", po::value<TrackType>(&parameters.trackType)->default_value(parameters.trackType),
				"The type of track to use. Allowed values: circle, zigzag, curvy, random")
		("min-track-width", po::value<float>(&parameters.minRandomTrackWidth)->default_value(parameters.minRandomTrackWidth),
				"Minimum track width for randomly generated tracks")
		("max-track-width", po::value<float>(&parameters.maxRandomTrackWidth)->default_value(parameters.maxRandomTrackWidth),
				"Maximum track width for randomly generated tracks")
		("track-seed", po::value<uint>(&parameters.randomTrackSeed)->default_value(parameters.randomTrackSeed),
				"Seed for randomly generated tracks")
		("track-points", po::value<int>(&parameters.randomTrackPoints)->default_value(parameters.randomTrackPoints),
				"Number of points for randomly generated tracks")
		("fps-limit", po::value<int>(&parameters.fpsLimit)->default_value(parameters.fpsLimit),
				"set fps limit. negative value means no limit")
	;

	po::options_description commandLineDescription("Options");
	commandLineDescription.add(commandLineOnlyDescription);
	commandLineDescription.add(configFileDescription);

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, commandLineDescription), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << commandLineDescription << std::endl;
		exit(0);
	}

	parameters.isTrainingAI = vm.count("ai");

	if (vm.count("config")) {
		po::store(po::parse_config_file<char>(vm["config"].as<std::string>().c_str(), configFileDescription), vm);
		po::notify(vm);
	}

	if (vm.count("neural-network")) {
		parameters.neuralNetworkFile = vm["neural-network"].as<std::string>();
	}

	return parameters;
}

}

