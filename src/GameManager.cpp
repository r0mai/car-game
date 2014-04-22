
#include "GameManager.hpp"

#include <cassert>

namespace car {

GameManager::GameManager(std::function<Track()> trackCreator) {
	Track track = trackCreator();
	track.check();
	model.setCar(track.createCar());
	model.setTrack(track);
	rayPoints = model.getRayPoints(rayCount);
}

void GameManager::advance() {
	handleInput();
	model.advanceTime(physicsTimeStep);
	rayPoints = model.getRayPoints(rayCount);
}

void GameManager::setNeuralNetwork(const NeuralNetwork& network) {
	assert(network.getOutputNeuronCount() == 3);
	assert(network.getInputNeuronCount() > 0);

	neuralNetwork = network;
	rayCount = neuralNetwork.getInputNeuronCount() - 1;
}

void GameManager::handleInput() {
	handleUserInput();
	if ( isAIControl ) {
		Weights outputs = callNeuralNetwork();
		assert(outputs.size() == 3);

		Car& car = model.getCar();

		car.setThrottle((outputs[0] + 1)/2.f);
		car.setBrake((outputs[1] + 1)/2.f);
		car.setTurnLevel(outputs[2]);
	}
}

void GameManager::handleUserInput() {}

Weights GameManager::callNeuralNetwork() {
	Weights inputs(rayCount + 1);

	const float sigmoidDamping = 5.f;

	const sf::Vector2f& carPosition = model.getCar().getPosition();
	for (unsigned i = 0; i < rayCount; ++i) {
		auto rayPoint = rayPoints[i];
		if (rayPoint) {
			float distance = getDistance(carPosition, *rayPoint);
			inputs[i] = sigmoidApproximation(distance/sigmoidDamping);
		} else {
			inputs[i] = 1.f;
		}
	}
	inputs.back() = sigmoidApproximation(model.getCar().getSpeed());

	return neuralNetwork.evaluateInput(inputs);
}

}

