
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(const Parameters& parameters, std::function<Track()> trackCreator) :
	GameManager(parameters, trackCreator) {}


void AIGameManager::run() {
	while (!stopCondition()) {
		advance();
	}
}

float AIGameManager::getFitness() const {
	//TODO this will need some more work probably
	auto checkpoints = model.getTrack().getNumberOfCheckpoints();
	auto crossedCheckpoints = model.getNumberOfCrossedCheckpoints();

	const float checkpointMultiplier = 100.f;
	const float lateCheckpointMultiplier = 2.f;
	const float distanceMultiplier = 0.5f;

	float result = model.getCar().getTravelDistance() * distanceMultiplier;

	if (crossedCheckpoints > checkpoints) {
		result += checkpoints * checkpointMultiplier +
				(crossedCheckpoints - checkpointMultiplier) * lateCheckpointMultiplier;
	} else {
		result += crossedCheckpoints * checkpointMultiplier;
	}

	return result;;
}

bool AIGameManager::stopCondition() const {
	return model.hasCarCollided() || model.getCurrentTime() > maxTime;
}

}

