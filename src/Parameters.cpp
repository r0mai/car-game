
#include "Parameters.hpp"

#include <stdexcept>
#include <iostream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "Track/TrackArgumentParser.hpp"
#include "PrefixMap.hpp"
#include "LazyArgumentMap.hpp"
#include "StringEnumValue.hpp"

namespace car {

LAZY_ARGUMENT_PREFIX_MAP(PanMode, panModes) {
	return {
		STRING_ENUM_VALUE(PanMode, center),
		STRING_ENUM_VALUE(PanMode, fit),
	};
}

LAZY_ARGUMENT_PREFIX_MAP(GameType, gameTypes) {
	return {
		STRING_ENUM_VALUE(GameType, realtime),
		STRING_ENUM_VALUE(GameType, learning),
		STRING_ENUM_VALUE(GameType, benchmark),
	};
}

template <typename T>
std::string argumentValues(const PrefixMap<T>& map) {
	std::vector<std::string> values;
	for (const auto& value: map) {
		values.push_back(value.first);
	}
	return boost::algorithm::join(values, ", ");
}

std::istream& operator>>(std::istream& is, PanMode& panMode) {
	std::string s;
	is >> s;
	panMode = panModes().at(s);

	return is;
}

std::ostream& operator<<(std::ostream& os, PanMode panMode) {
	switch (panMode) {
	case PanMode::center: return os << "center";
	case PanMode::fit: return os << "fit";
	default: return os;
	}
}

std::istream& operator>>(std::istream& is, GameType& gameType) {
	std::string s;
	is >> s;
	gameType = gameTypes().at(s);

	return is;
}

std::ostream& operator<<(std::ostream& os, GameType panMode) {
	switch (panMode) {
	case GameType::realtime: return os << "realtime";
	case GameType::learning: return os << "learning";
	case GameType::benchmark: return os << "benchmark";
	default: return os;
	}
}

Parameters parseParameters(int argc, char **argv) {

	namespace po = boost::program_options;

	Parameters parameters;

	std::string binaryLocation = argv[0];
	boost::trim_right_if(binaryLocation, !boost::is_any_of("/"));

	parameters.projectRootPath = binaryLocation + "../";

	std::vector<std::string> configFiles;

	std::string gameTypeDescription = "The type of game to run. Allowed values: " + argumentValues(gameTypes());

	po::options_description commandLineOnlyDescription("Command-line only options");
	commandLineOnlyDescription.add_options()
		("help", "produce help message")
		("game-type", po::value(&parameters.gameType)->default_value(parameters.gameType),
				gameTypeDescription.c_str())
		("config", po::value<std::vector<std::string>>(&configFiles),
				"Reads configuration parameters from the specified file. It can be given multiple times. "
				"Newer values override older ones.")
	;

	po::options_description configFileDescription("Command-line and config file options");

	std::string panModeDescription = "Set panning mode. Allowed values: " + argumentValues(panModes());

	configFileDescription.add_options()
		("seed", po::value<int>(),
				"Seed used for random number generation (e.g. for population generation). Default is to use random seed.")
		("population-size", po::value<unsigned>(&parameters.populationSize)->default_value(parameters.populationSize),
				"Size of the population used in the genetic algorithm.")
		("generation-limit", po::value<unsigned>(),
				"Exit after this many generations. Default is to never exit.")
		("printout-frequency", po::value(&parameters.printoutFrequency)->default_value(parameters.printoutFrequency),
				"The number of generations after which output is printed")
		("hidden-layer-count", po::value<unsigned>(&parameters.hiddenLayerCount)->default_value(parameters.hiddenLayerCount),
				"Number of hidden layers in the nerual network.")
		("neuron-per-hidden-layer", po::value<unsigned>(&parameters.neuronPerHiddenLayer)->default_value(parameters.neuronPerHiddenLayer),
				"Number of neurons/hidden layer in the nerual network.")
		("use-recurrence", "Use recurrence for the neurons")
		("ray-count", po::value<unsigned>(&parameters.rayCount)->default_value(parameters.rayCount),
				"Number of rays providing information to the car.")
		("neural-network", po::value<std::string>(),
				"Load neural-network from file.")
		("output-ai,o", po::value<std::string>(&parameters.bestAIFile)->default_value(parameters.bestAIFile),
				"Specifies where to save the best trained AI.")
		("output-population", po::value<std::string>(),
				"Specifies where to save the current population.")
		("input-population", po::value<std::string>(),
				"Load population from file.")
		("track", po::value<std::vector<std::string>>(&parameters.tracks),
				"The type of track to use. It can be given multiple times. "
				"For AI learning, use all tracks for learning. "
				"For real time simulation, use only the first.\n"
				"Format: filename[:arg1[:arg2[:...]]]")
		("threads,j", po::value<unsigned>(&parameters.threadCount)->default_value(parameters.threadCount),
				"Number of threads used for population simulation.")
		("starting-populations", po::value<unsigned>(&parameters.startingPopulations)->default_value(parameters.startingPopulations),
				"The number of independent populations to start the learning with.")
		("population-cutoff", po::value<unsigned>(&parameters.populationCutoff)->default_value(parameters.populationCutoff),
				"The number of generations after the worst population is dropped (if there are more than one).")
		("fitness-function", po::value<MathExpression>(&parameters.fitnessExpression)->default_value(parameters.fitnessExpression),
				"Fitness function.")
		("physics-frequency", po::value<unsigned>(&parameters.physicsTimeStepsPerSecond)->default_value(parameters.physicsTimeStepsPerSecond),
				"Specifies how many times per second the physics should be recalculated.")
		("fps-limit", po::value<int>(&parameters.fpsLimit)->default_value(parameters.fpsLimit),
				"Set fps limit. Negative value means no limit.")
		("screen-width", po::value<unsigned>(&parameters.screenWidth)->default_value(parameters.screenWidth),
				"Screen width for rendering,")
		("screen-height", po::value<unsigned>(&parameters.screenHeight)->default_value(parameters.screenHeight),
				"Screen height for rendering.")
		("min-pixels-per-meter", po::value(&parameters.minPixelsPerMeter)->default_value(parameters.minPixelsPerMeter),
				"Minimum resolution of the view.")
		("max-pixels-per-meter", po::value(&parameters.maxPixelsPerMeter)->default_value(parameters.maxPixelsPerMeter),
				"Maximum resolution of the view.")
		("pan-mode", po::value<PanMode>(&parameters.panMode)->default_value(parameters.panMode),
				panModeDescription.c_str())
		("pan-threshold", po::value<std::string>(),
				"The maximum amount that the car moves from the screen center before the screen gets panned. The value is in meter (m), pixels (px) or percent (%).")
	;

	po::options_description commandLineDescription("Options");
	commandLineDescription.add(commandLineOnlyDescription);
	commandLineDescription.add(configFileDescription);

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, commandLineDescription), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << commandLineDescription << "\n\n" << track::trackArgumentParser::getHelpString() << std::endl;
		std::exit(0);
	}

	parameters.useRecurrence = vm.count("use-recurrence");

	// Boost only considers the first config value, but we want it the other way around
	// so the config files are read in reverse order. Now the new values override the
	// old ones.
	for (const auto& configFile: configFiles | boost::adaptors::reversed) {
		po::store(po::parse_config_file<char>(configFile.c_str(), configFileDescription), vm);
		po::notify(vm);
	}

	if (vm.count("neural-network")) {
		parameters.neuralNetworkFile = vm["neural-network"].as<std::string>();
	}
	if (vm.count("generation-limit")) {
		parameters.generationLimit = vm["generation-limit"].as<unsigned>();
	}
	if (vm.count("output-population")) {
		parameters.populationOutputFile = vm["output-population"].as<std::string>();
	}
	if (vm.count("input-population")) {
		parameters.populationInputFile = vm["input-population"].as<std::string>();
	}

	if (vm.count("pan-threshold")) {
		parameters.panThreshold = parseScreenDimenstion(vm["pan-threshold"].as<std::string>());
	}

	if (parameters.minPixelsPerMeter < 0.f) {
		throw std::logic_error{"Pixels per width cannot be negative"};
	}
	if (parameters.maxPixelsPerMeter < 0.f) {
		throw std::logic_error{"Pixels per width cannot be negative"};
	}
	if (parameters.minPixelsPerMeter > parameters.maxPixelsPerMeter) {
		throw std::logic_error{"Minimum pixels per meter cannot be larger than maximum"};
	}

	if (vm.count("seed")) {
		std::srand(vm["seed"].as<int>());
	} else {
		std::srand(std::time(0));
	}


	return parameters;
}

}

