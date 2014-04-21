
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager() {
	model.setCar(Car(sf::Vector2f(0, 55)));
	model.setTrack(createCircleTrack());
}

void AIGameManager::run() {
	std::clock_t start = clock();
	for (int i = 0; i < 10000; ++i) {
		//TODO meaningful inputs
		Weights inputs;
		std::generate_n(std::back_inserter(inputs), 10, [] { return randomReal(0, 1); });

		Weights outputs = neuralNetwork.evaluateInput(inputs);
		assert(outputs.size() == 4);

		model.setForwardPressed(outputs[0] > 0.5);
		model.setBackwardPressed(outputs[1] > 0.5);
		model.setLeftPressed(outputs[2] > 0.5);
		model.setRightPressed(outputs[3] > 0.5);

		model.advanceTime(physicsTimeStep);
	}
	std::cout << "Time taken for 1 iteration: " << std::clock() - start << std::endl;
}

}

