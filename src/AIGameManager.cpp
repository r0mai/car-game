
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(const CommonParameters& parameters, track::TrackCreator trackCreator, const MathExpression& fitnessExpression) :
	gameManager(parameters, trackCreator),
	fitnessExpression{fitnessExpression} {}

void AIGameManager::init() {
	gameManager.init();
}

void AIGameManager::run() {
	while (!stopCondition()) {
		gameManager.advance();
	}
}

float AIGameManager::getFitness() const {
	auto& model = gameManager.getModel();

	SymbolTable symbolTable = {
		{"td", model.getCar().getTravelDistance()},
		{"cps", model.getTrack().getNumberOfCheckpoints()},
		{"ccps", model.getNumberOfCrossedCheckpoints()}
	};

	return evaluateMathExpression(fitnessExpression, symbolTable);
}

bool AIGameManager::stopCondition() const {
	auto& model = gameManager.getModel();

	return model.hasCarCollided() || model.getCurrentTime() > maxTime;
}

}

