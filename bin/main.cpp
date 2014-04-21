
#include "RealTimeGameManager.hpp"
#include "AIGameManager.hpp"
#include "TrackType.hpp"
#include "Track.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/program_options.hpp>

using namespace car;

int main(int argc, char **argv) {

	namespace po = boost::program_options;

	TrackType trackType = TrackType::circle;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("ai", "simulate AI")
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
		manager.run();
	}
	return 0;
}

