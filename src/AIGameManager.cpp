
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(const CommonParameters& parameters, track::TrackCreator trackCreator, const MathExpression& fitnessExpression) :
	GameManager(parameters, trackCreator),
	fitnessExpression{fitnessExpression} {}


void AIGameManager::run() {
	while (!stopCondition()) {
		advance();
	}
}

float AIGameManager::getFitness() const {
	SymbolTable symbolTable = {
		{"td", model.getCar().getTravelDistance()},
		{"cps", model.getTrack().getNumberOfCheckpoints()},
		{"ccps", model.getNumberOfCrossedCheckpoints()}
	};

	return evaluateMathExpression(fitnessExpression, symbolTable);
}

bool AIGameManager::stopCondition() const {
	return model.hasCarCollided() || model.getCurrentTime() > maxTime;
}

}

