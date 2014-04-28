
#include "RealTimeGameManager.hpp"
#include "NeuralController.hpp"
#include "Parameters.hpp"
#include "Track.hpp"
#include "ThreadPool.hpp"
#include "TrackCreator.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>

using namespace car;

int main(int argc, char **argv) {

	Parameters parameters = parseParameters(argc, argv);

	TrackCreator trackCreator{parameters};
	std::vector<std::function<Track()>> trackCreators = trackCreator(parameters.tracks);

	if (parameters.isTrainingAI) {
		ThreadPool threadPool;
		threadPool.setNumThreads(parameters.threadCount);
		ThreadPoolRunner runner{threadPool};
		NeuralController controller{parameters, trackCreators, threadPool.getIoService()};
		controller.run();
	} else {
		RealTimeGameManager manager{parameters, trackCreators[0]};

		manager.setFPSLimit(parameters.fpsLimit);

		if (parameters.neuralNetworkFile) {
			NeuralNetwork network;

			std::ifstream ifs(*parameters.neuralNetworkFile);
			boost::archive::text_iarchive ia(ifs);
			ia >> network;

			manager.setNeuralNetwork(network);
		}
		manager.run();
	}
	return 0;
}

