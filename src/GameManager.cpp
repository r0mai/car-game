
#include "GameManager.hpp"

#include <cassert>

namespace car {

GameManager::GameManager(const Parameters& parameters, std::function<Track()> trackCreator) :
	parameters(parameters)
{
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

		float throttleOutput = clamp((2.f/3.f)*outputs[0] + (2.f/3.f), 0.f, 1.f);
		float brakeOutput = clamp((2.f/3.f)*outputs[1] + (1.f/3.f), 0.f, 1.f);
		float turnLevelOutput = outputs[2];

		car.setThrottle(throttleOutput);
		car.setBrake(brakeOutput);
		car.setTurnLevel(turnLevelOutput);
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

