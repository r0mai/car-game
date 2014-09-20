
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(const CommonParameters& commonParameters,
		const IterationParameters& iterationParameters,
		std::shared_ptr<const track::Track> track) :
	gameManager(commonParameters, std::move(track)),
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

