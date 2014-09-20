
#include "RealTimeGameManager.hpp"
#include "LearningController.hpp"
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
	std::vector<std::shared_ptr<const track::Track>> tracks;
	tracks.reserve(trackCreators.size());
	for (auto& trackCreator: trackCreators) {
		tracks.push_back(std::make_shared<track::Track>(trackCreator()));
		tracks.back()->check();
	}

	switch (parameters.gameType) {
	case GameType::learning: {
		ThreadPool threadPool;
		threadPool.setNumThreads(parameters.learningParameters.threadCount);
		ThreadPoolRunner runner{threadPool};
		LearningController controller{parameters.learningParameters, tracks, threadPool.getIoService()};
		controller.run();
		break;
	 }
	case GameType::realtime: {
		RealTimeGameManager manager{parameters.realTimeParameters,
			tracks[0]};

		manager.run();
		break;
	 }
	case GameType::benchmark: {
		Benchmark benchmark{parameters.benchmarkParameters, tracks};

		benchmark.run();
		break;

	  }
	}
	return 0;
}

