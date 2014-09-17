
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(const CommonParameters& commonParameters,
		const IterationParameters& iterationParameters,
		track::TrackCreator trackCreator) :
	gameManager(commonParameters, trackCreator),
	iterationParameters(iterationParameters)
{}

void AIGameManager::init() {
	gameManager.init();
}

void AIGameManager::run() {
	while (!stopCondition()) {
		gameManager.advance();
	}
}

bool AIGameManager::stopCondition() const {
	auto& model = gameManager.getModel();

	return model.hasCarCollided() || model.getCurrentTime() > iterationParameters.timeLimit;
}

}

