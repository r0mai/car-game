
#include "RealTimeGameManager.hpp"
#include "NeuralController.hpp"
#include "Parameters.hpp"
#include "TrackType.hpp"
#include "Track.hpp"
#include "RandomTrackGenerator.hpp"
#include "PolygonTrackBuilder.hpp"
#include "ThreadPool.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>

using namespace car;

int main(int argc, char **argv) {

	Parameters parameters = parseParameters(argc, argv);

	std::function<Track()> trackCreator;
	std::vector<std::function<Track()>> trackCreators;

	RandomTrackGenerator generator{PolygonTrackBuilder{5.f}, 100,
			parameters.randomTrackPoints, parameters.minRandomTrackWidth, parameters.maxRandomTrackWidth,
			{-60.f, -60.f}, {60.f, 60.f}};

	switch (parameters.trackType) {
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
		if (parameters.randomTrackSeed.size() == 0) {
			trackCreator = std::bind(generator, std::rand());
		} else {
			for (auto seed: parameters.randomTrackSeed) {
				trackCreators.push_back(std::bind(generator, seed));
			}
		}
		break;
	}

	if (trackCreators.size() == 0) {
		trackCreators.push_back(trackCreator);
	}

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

