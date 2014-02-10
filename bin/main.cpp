
#include "RealTimeGameManager.hpp"
#include "AIGameManager.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include <boost/program_options.hpp>

int main(int argc, char **argv) {

	namespace po = boost::program_options;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("ai", "simulate AI")
		("fps-limit", po::value<int>()->default_value(-1), "set fps limit. negative value means no limit")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 1;
	}

	std::srand(std::time(0));
	if (vm.count("ai")) {
		car::AIGameManager manager;
		manager.run();
	} else {
		car::RealTimeGameManager manager;
		manager.setFPSLimit(vm["fps-limit"].as<int>());
		manager.run();
	}
	return 0;
}

