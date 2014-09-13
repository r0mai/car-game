
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(const CommonParameters& parameters,
		track::TrackCreator trackCreator) :
	gameManager(parameters, trackCreator)
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

	return model.hasCarCollided() || model.getCurrentTime() > maxTime;
}

}

