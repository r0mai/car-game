
#include "Parameters.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

namespace car {

Parameters parseParameters(int argc, char **argv) {

	namespace po = boost::program_options;

	Parameters parameters;

	std::string binaryLocation = argv[0];
	boost::trim_right_if(binaryLocation, !boost::is_any_of("/"));

	parameters.projectRootPath = binaryLocation + "../";

	std::string configFile;

	po::options_description commandLineOnlyDescription("Command-line only options");
	commandLineOnlyDescription.add_options()
		("help", "produce help message")
		("ai", "train AI")
		("config", po::value<std::string>(), "reads configuration parameters from the specified file")
	;

	po::options_description configFileDescription("Command-line and config file options");
	configFileDescription.add_options()
		("seed", po::value<int>(),
				"Seed used for random number generation (e.g. for population generation). Default is to use random seed.")
		("population-size", po::value<unsigned>(&parameters.populationSize)->default_value(parameters.populationSize),
				"Size of the population used in the genetic algorithm.")
		("generation-limit", po::value<unsigned>(),
				"Exit after this many generations. Default is to never exit.")
		("hidden-layer-count", po::value<unsigned>(&parameters.hiddenLayerCount)->default_value(parameters.hiddenLayerCount),
				"Number of hidden layers in the nerual network.")
		("neuron-per-hidden-layer", po::value<unsigned>(&parameters.neuronPerHiddenLayer)->default_value(parameters.neuronPerHiddenLayer),
				"Number of neurons/hidden layer in the nerual network.")
		("neural-network", po::value<std::string>(),
				"Load neural-network from file.")
		("output-ai,o", po::value<std::string>(&parameters.bestAIFile)->default_value(parameters.bestAIFile),
				"Specifies where to save the best trained AI.")
		("track", po::value<TrackType>(&parameters.trackType)->default_value(parameters.trackType),
				"The type of track to use. Allowed values: circle, zigzag, curvy, random")
		("min-track-width", po::value<float>(&parameters.minRandomTrackWidth)->default_value(parameters.minRandomTrackWidth),
				"Minimum track width for randomly generated tracks.")
		("max-track-width", po::value<float>(&parameters.maxRandomTrackWidth)->default_value(parameters.maxRandomTrackWidth),
				"Maximum track width for randomly generated tracks.")
		("track-seed", po::value<uint>(&parameters.randomTrackSeed)->default_value(parameters.randomTrackSeed),
				"Seed for randomly generated tracks.")
		("track-points", po::value<int>(&parameters.randomTrackPoints)->default_value(parameters.randomTrackPoints),
				"Number of points for randomly generated tracks.")
		("threads", po::value<std::size_t>(&parameters.threadCount)->default_value(parameters.threadCount),
				"Number of threads used for population simulation.")
		("fps-limit", po::value<int>(&parameters.fpsLimit)->default_value(parameters.fpsLimit),
				"Set fps limit. Negative value means no limit.")
	;

	po::options_description commandLineDescription("Options");
	commandLineDescription.add(commandLineOnlyDescription);
	commandLineDescription.add(configFileDescription);

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, commandLineDescription), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << commandLineDescription << std::endl;
		std::exit(0);
	}

	parameters.isTrainingAI = vm.count("ai");

	if (vm.count("config")) {
		po::store(po::parse_config_file<char>(vm["config"].as<std::string>().c_str(), configFileDescription), vm);
		po::notify(vm);
	}

	if (vm.count("neural-network")) {
		parameters.neuralNetworkFile = vm["neural-network"].as<std::string>();
	}
	if (vm.count("generation-limit")) {
		parameters.generationLimit = vm["generation-limit"].as<unsigned>();
	}

	if (vm.count("seed")) {
		std::srand(vm["seed"].as<int>());
	} else {
		std::srand(std::time(0));
	}


	return parameters;
}

}

