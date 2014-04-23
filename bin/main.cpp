
#include "RealTimeGameManager.hpp"
#include "NeuralController.hpp"
#include "TrackType.hpp"
#include "Track.hpp"
#include "RandomTrackGenerator.hpp"
#include "PolygonTrackBuilder.hpp"

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

	TrackType trackType = TrackType::circle;

	float minTrackWidth = 5.f;
	float maxTrackWidth = 12.f;
	uint trackSeed = 0;
	int trackPoints = 10;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("ai", "simulate AI")
		("neural-network", po::value<std::string>(), "load neural-network from file")
		("track", po::value<TrackType>(&trackType)->default_value(trackType),
				"The type of track to use. Allowed values: circle, zigzag, curvy, random")
		("min-track-width", po::value<float>(&minTrackWidth)->default_value(minTrackWidth),
				"Minimum track width for randomly generated tracks")
		("max-track-width", po::value<float>(&maxTrackWidth)->default_value(maxTrackWidth),
				"Maximum track width for randomly generated tracks")
		("track-seed", po::value<uint>(&trackSeed)->default_value(trackSeed),
				"Seed for randomly generated tracks")
		("track-points", po::value<int>(&trackPoints)->default_value(trackPoints),
				"Number of points for randomly generated tracks")

		("fps-limit", po::value<int>()->default_value(-1), "set fps limit. negative value means no limit")

	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 1;
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
	case TrackType::random:
		RandomTrackGenerator generator{PolygonTrackBuilder{5.f}, 100,
				trackPoints, minTrackWidth, maxTrackWidth,
				{-60.f, -60.f}, {60.f, 60.f}};
		trackCreator = std::bind(generator, trackSeed);
		break;
	}

	std::srand(std::time(0));
	if (vm.count("ai")) {
		NeuralController controller{trackCreator};
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

