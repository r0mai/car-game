
#include "Parameters.hpp"

#include <stdexcept>
#include <iostream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "Track/TrackArgumentParser.hpp"
#include "util/PrefixMap.hpp"
#include "util/LazyArgumentMap.hpp"
#include "StringEnumValue.hpp"
#include "PerturbationParameters.hpp"

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
std::string argumentValues(const util::PrefixMap<T>& map) {
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

namespace po = boost::program_options;

void fillCarParameters(po::options_description& optionsDescription,
		CarParameters& parameters, const std::string& prefix) {
	optionsDescription.add_options()
		((prefix + "air-resistance").c_str(),
			po::value(&parameters.cDrag)
				->default_value(parameters.cDrag),
				"Drag ratio that is proportional to the square of the speed.")
		((prefix + "rolling-resistance").c_str(),
			po::value(&parameters.cRollingResistance)
				->default_value(parameters.cRollingResistance),
				"Drag ratio that is linearly proportional to the speed")
		((prefix + "engine-power").c_str(),
			po::value(&parameters.pEngine)
				->default_value(parameters.pEngine),
				"The power of the engine (under full throttle).")
		((prefix + "max-engine-force").c_str(),
			po::value(&parameters.fEngineMax)
				->default_value(parameters.fEngineMax),
				"The maximum force the engine can exert.")
		((prefix + "brake-force").c_str(),
			po::value(&parameters.fBrake)
				->default_value(parameters.fBrake),
				"")
		((prefix + "mass").c_str(),
			po::value(&parameters.mass)
				->default_value(parameters.mass),
				"The mass of the car.")
		((prefix + "max-turn-angle").c_str(),
			po::value(&parameters.maxTurnAngle)
				->default_value(parameters.maxTurnAngle),
				"Maximum turning angle of the wheel (under full steering)")
		((prefix + "turn-rate").c_str(),
			po::value(&parameters.turnRate)
				->default_value(parameters.turnRate),
				"The turn rate of the car (higher means better turning")
		((prefix + "rear-cm-distance").c_str(),
			po::value(&parameters.rearCMDistance)
				->default_value(parameters.rearCMDistance),
				"Distance of the rear of the car from the center of mass.")
		((prefix + "frontcm-distance").c_str(),
			po::value(&parameters.frontCMDistance)
				->default_value(parameters.frontCMDistance),
				"Distance of the front of the car from the center of mass.")
		((prefix + "width").c_str(),
			po::value(&parameters.carWidth)
				->default_value(parameters.carWidth),
				"Width of the car")
		((prefix + "throttle-increase").c_str(),
			po::value(&parameters.throttleIncreaseSpeed)
				->default_value(parameters.throttleIncreaseSpeed),
				"The rate with which the throttle level increases when forward is pressed in manual mode.")
		((prefix + "throttle.decrease").c_str(),
			po::value(&parameters.throttleDecreaseSpeed)
				->default_value(parameters.throttleDecreaseSpeed),
				"The rate with which the throttle level decreases when forward is releases in manual mode.")
		((prefix + "brake-increase").c_str(),
			po::value(&parameters.brakeIncreaseSpeed)
				->default_value(parameters.brakeIncreaseSpeed),
				"The rate with which the brake level increases when backward is pressed in manual mode.")
		((prefix + "brake-decrease").c_str(),
			po::value(&parameters.brakeDecreaseSpeed)
				->default_value(parameters.brakeDecreaseSpeed),
				"The rate with which the brake level decreases when backward is releases in manual mode.")
		((prefix + "turn-speed").c_str(),
			po::value(&parameters.turnSpeed)
				->default_value(parameters.turnSpeed),
				"The rate with which the turn level changes when left/right is pressed/released in manual mode.")
		;
}

Parameters parseParameters(int argc, char **argv) {


	Parameters parameters;
	CommonParameters commonParameters;
	IterationParameters iterationParameters;
	CarInputParameters carInputParameters;

	std::string binaryLocation = argv[0];
	boost::trim_right_if(binaryLocation, !boost::is_any_of("/"));

	parameters.realTimeParameters.projectRootPath = binaryLocation + "../";

	std::vector<std::string> configFiles;

	std::string gameTypeDescription = "The type of game to run. Allowed values: " + argumentValues(gameTypes());

	po::options_description commandLineOnlyDescription("Command-line only options");
	commandLineOnlyDescription.add_options()
		("help,h", "Print help for command line options")
		("help-random", "Print help for possible options in random track config files")
		("game-type", po::value(&parameters.gameType)->default_value(parameters.gameType),
				gameTypeDescription.c_str())
		("config", po::value<std::vector<std::string>>(&configFiles),
				"Reads configuration parameters from the specified file. It can be given multiple times. "
				"Newer values override older ones.")
	;

	po::options_description configFileDescription("Command-line and config file options");
	po::options_description commonDescription("Options for all game types");
	commonDescription.add_options()
		("ray-count",
			po::value(&commonParameters.rayCount)
				->default_value(commonParameters.rayCount),
			"Number of rays providing information to the car.")
		("checkpoint-look-ahead",
			po::value(&commonParameters.checkpointLookAhead)
				->default_value(commonParameters.checkpointLookAhead),
			"The number of checkpoints to give as an input to the neural network.")
		("physics-frequency",
			po::value(&commonParameters.physicsTimeStepsPerSecond)
				->default_value(commonParameters.physicsTimeStepsPerSecond),
			"Specifies how many times per second the physics should be recalculated.")
		("track",
			po::value(&parameters.tracks),
			"The type of track to use. It can be given multiple times. "
			"For AI learning, use all tracks for learning. "
			"For real time simulation, use only the first.\n"
			"Format: filename[:arg1[:arg2[:...]]]")
		("seed",
			po::value<int>(),
			"Seed used for random number generation (e.g. for population generation). Default is to use random seed.")
		;

	po::options_description carDescription("Options for the car (used in all game types)");
	fillCarParameters(carDescription, commonParameters.carParameters, "car.");

	po::options_description perturbationDescription("Options for the perturbation of the car (used in all game types, but really useful in learning mode). These control the sigma value of the normal distribution used for the perturbation");
	perturbationDescription.add_options()
		("perturbation.seed",
			po::value(&commonParameters.perturbationParameters.seed),
			"Seed that controls perturbation of car parameters. If not given, a random value is used (controlled by the --seed option).")
		;
	fillCarParameters(perturbationDescription, commonParameters.perturbationParameters, "perturbation.");

	po::options_description iterationDescription("Options for learning and benchmark game types");
	iterationDescription.add_options()
		("generation-limit",
			po::value<unsigned>(),
			"Exit after this many generations. Default is to never exit.")
		("time-limit",
			po::value(&iterationParameters.timeLimit)->default_value(iterationParameters.timeLimit),
			"Time limit for the iteration in seconds. Iteration ends if car does not leave track until this time")
		("printout-frequency",
			po::value(&iterationParameters.printoutFrequency)
				->default_value(iterationParameters.printoutFrequency),
			"The number of generations after which output is printed")
		("fitness-script",
			po::value(&iterationParameters.fitnessScript)
				->default_value(iterationParameters.fitnessScript),
			"Lua script used for calculating fitness.")
		;

	po::options_description carInputDescription("Options for realtime and benchmark game types");
	carInputDescription.add_options()
		("neural-network", po::value(&carInputParameters.neuralNetworkFile),
			"Load neural networks from file. For benchmark mode, at least 1 must be given.")
		;

	po::options_description realtimeDescription("Options for realtime game type");
	std::string panModeDescription = "Set panning mode. Allowed values: " + argumentValues(panModes());
	realtimeDescription.add_options()
		("fps-limit",
			po::value(&parameters.realTimeParameters.fpsLimit)
				->default_value(parameters.realTimeParameters.fpsLimit),
			"Set fps limit. Negative value means no limit.")
		("screen-width",
			po::value(&parameters.realTimeParameters.screenWidth)
				->default_value(parameters.realTimeParameters.screenWidth),
			"Screen width for rendering,")
		("screen-height",
			po::value(&parameters.realTimeParameters.screenHeight)
				->default_value(parameters.realTimeParameters.screenHeight),
			"Screen height for rendering.")
		("min-pixels-per-meter",
			po::value(&parameters.realTimeParameters.minPixelsPerMeter)
				->default_value(parameters.realTimeParameters.minPixelsPerMeter),
			"Minimum resolution of the view.")
		("max-pixels-per-meter",
			po::value(&parameters.realTimeParameters.maxPixelsPerMeter)
				->default_value(parameters.realTimeParameters.maxPixelsPerMeter),
			"Maximum resolution of the view.")
		("pan-mode",
			po::value(&parameters.realTimeParameters.panMode)
				->default_value(parameters.realTimeParameters.panMode),
				panModeDescription.c_str())
		("pan-threshold",
			po::value<std::string>(),
			"The maximum amount that the car moves from the screen center before the screen gets panned. The value is in meter (m), pixels (px) or percent (%).")
		("car-out-timeout",
			po::value(&parameters.realTimeParameters.carOutTimeout)
				->default_value(parameters.realTimeParameters.carOutTimeout),
			"The amount of time spent outside the track after the car turns completely red")
		("trace-output-interval",
			po::value(&parameters.realTimeParameters.traceOutputInterval)
				->default_value(parameters.realTimeParameters.traceOutputInterval),
			"The interval in meters after a new point in the trace line is printed.");

	po::options_description learningDescription("Options for learning game type");
	learningDescription.add_options()
		("hidden-layer-count",
			po::value(&parameters.learningParameters.hiddenLayerCount)
				->default_value(parameters.learningParameters.hiddenLayerCount),
			"Number of hidden layers in the nerual network.")
		("neuron-per-hidden-layer",
			po::value(&parameters.learningParameters.neuronPerHiddenLayer)
				->default_value(parameters.learningParameters.neuronPerHiddenLayer),
			"Number of neurons/hidden layer in the nerual network.")
		("use-recurrence",
			"Use recurrence for the neurons")
		("population-size",
			po::value(&parameters.learningParameters.populationSize)
				->default_value(parameters.learningParameters.populationSize),
			"Size of the population used in the genetic algorithm.")
		("output-ai,o", po::value(&parameters.learningParameters.bestAIFile)
				->default_value(parameters.learningParameters.bestAIFile),
			"Specifies where to save the best trained AI.")
		("output-population",
			po::value<std::string>(),
			"Specifies where to save the current population.")
		("input-population",
			po::value<std::string>(),
			"Load population from file.")
		("threads,j", po::value(&parameters.learningParameters.threadCount)
				->default_value(parameters.learningParameters.threadCount),
			"Number of threads used for population simulation.")
		("starting-populations",
			po::value(&parameters.learningParameters.startingPopulations)
				->default_value(parameters.learningParameters.startingPopulations),
			"The number of independent populations to start the learning with.")
		("population-cutoff",
			po::value(&parameters.learningParameters.populationCutoff)
				->default_value(parameters.learningParameters.populationCutoff),
			"The number of generations after the worst population is dropped (if there are more than one).")
		;

	configFileDescription.add(commonDescription);
	configFileDescription.add(carDescription);
	configFileDescription.add(perturbationDescription);
	configFileDescription.add(iterationDescription);
	configFileDescription.add(carInputDescription);
	configFileDescription.add(realtimeDescription);
	configFileDescription.add(learningDescription);

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

	if (vm.count("help-random")) {
		std::cout << track::trackArgumentParser::getHelpString() << std::endl;
		std::exit(0);
	}

	// Boost only considers the first config value, but we want it the other way around
	// so the config files are read in reverse order. Now the new values override the
	// old ones.
	for (const auto& configFile: configFiles | boost::adaptors::reversed) {
		po::store(po::parse_config_file<char>(configFile.c_str(), configFileDescription), vm);
		po::notify(vm);
	}

	parameters.learningParameters.useRecurrence = vm.count("use-recurrence");

	if (vm.count("generation-limit")) {
		iterationParameters.generationLimit = vm["generation-limit"].as<unsigned>();
	}
	if (vm.count("output-population")) {
		parameters.learningParameters.populationOutputFile = vm["output-population"].as<std::string>();
	}
	if (vm.count("input-population")) {
		parameters.learningParameters.populationInputFile = vm["input-population"].as<std::string>();
	}

	if (vm.count("pan-threshold")) {
		parameters.realTimeParameters.panThreshold = parseScreenDimenstion(vm["pan-threshold"].as<std::string>());
	}

	if (parameters.realTimeParameters.minPixelsPerMeter < 0.f) {
		throw std::logic_error{"Pixels per width cannot be negative"};
	}
	if (parameters.realTimeParameters.maxPixelsPerMeter < 0.f) {
		throw std::logic_error{"Pixels per width cannot be negative"};
	}
	if (parameters.realTimeParameters.minPixelsPerMeter > parameters.realTimeParameters.maxPixelsPerMeter) {
		throw std::logic_error{"Minimum pixels per meter cannot be larger than maximum"};
	}

	if (vm.count("seed")) {
		std::srand(vm["seed"].as<int>());
	} else {
		std::srand(std::time(0));
	}

	if (!vm.count("perturbation.seed")) {
		commonParameters.perturbationParameters.seed = std::rand();
	}

	parameters.realTimeParameters.commonParameters = commonParameters;
	parameters.realTimeParameters.carInputParameters = carInputParameters;
	parameters.learningParameters.commonParameters = commonParameters;
	parameters.learningParameters.iterationParameters = iterationParameters;
	parameters.benchmarkParameters.commonParameters = commonParameters;
	parameters.benchmarkParameters.iterationParameters = iterationParameters;
	parameters.benchmarkParameters.carInputParameters = carInputParameters;

	return parameters;
}

}

