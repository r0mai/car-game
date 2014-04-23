
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(std::function<Track()> trackCreator) :
	GameManager(trackCreator) {}


void AIGameManager::run() {
	while (!stopCondition()) {
		advance();
	}
}

float AIGameManager::getFitness() const {
	//TODO this will need some more work probably
	return 100.f * model.getNumberOfCrossedCheckpoints() + model.getCar().getTravelDistance();
}

bool AIGameManager::stopCondition() const {
	return model.hasCarCollided() || model.getCurrentTime() > maxTime;
}

}

