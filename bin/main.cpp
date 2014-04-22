
#include "RealTimeGameManager.hpp"
#include "AIGameManager.hpp"
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

	TrackType trackType = TrackType::circle;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("ai", "simulate AI")
		("neural-network", po::value<std::string>(), "load neural-network from file")
		("track", po::value<TrackType>(&trackType)->default_value(TrackType::circle),
				"The type of track to use. Allowed values: circle, zigzag")
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
	}

	std::srand(std::time(0));
	if (vm.count("ai")) {
		AIGameManager manager{trackCreator};
		manager.run();
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

