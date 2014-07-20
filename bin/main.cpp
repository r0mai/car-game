
#include "RealTimeGameManager.hpp"
#include "NeuralController.hpp"
#include "Parameters.hpp"
#include "ThreadPool.hpp"
#include "Track/Track.hpp"
#include "Track/TrackCreator.hpp"
#include "Track/TrackArgumentParser.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>

using namespace car;

int main(int argc, char **argv) {

	Parameters parameters = parseParameters(argc, argv);

	track::TrackCreators trackCreators =
			track::trackArgumentParser::parseArguments(parameters.tracks);

	switch (parameters.gameType) {
	case GameType::learning: {
		ThreadPool threadPool;
		threadPool.setNumThreads(parameters.threadCount);
		ThreadPoolRunner runner{threadPool};
		NeuralController controller{parameters, trackCreators, threadPool.getIoService()};
		controller.run();
		break;
	 }
	case GameType::realtime: {
		RealTimeGameManager manager{parameters, trackCreators[0], parameters.neuralNetworkFile};

		manager.setFPSLimit(parameters.fpsLimit);

		if (parameters.neuralNetworkFile) {
			NeuralNetwork network;

			std::ifstream ifs(*parameters.neuralNetworkFile);
			boost::archive::text_iarchive ia(ifs);
			ia >> network;

			manager.setNeuralNetwork(network);
		}
		manager.run();
		break;
	 }
	}
	return 0;
}

