
#include "RealTimeGameManager.hpp"
#include "NeuralController.hpp"
#include "Parameters.hpp"
#include "ThreadPool.hpp"
#include "Track/Track.hpp"
#include "Track/TrackCreator.hpp"
#include "Track/TrackArgumentParser.hpp"
#include "Benchmark.hpp"

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
		threadPool.setNumThreads(parameters.learningParameters.threadCount);
		ThreadPoolRunner runner{threadPool};
		NeuralController controller{parameters.learningParameters, trackCreators, threadPool.getIoService()};
		controller.run();
		break;
	 }
	case GameType::realtime: {
		RealTimeGameManager manager{parameters.realTimeParameters,
			trackCreators[0]};

		manager.run();
		break;
	 }
	case GameType::benchmark: {
		Benchmark benchmark{parameters.benchmarkParameters, trackCreators};

		benchmark.run();
		break;

	  }
	}
	return 0;
}

