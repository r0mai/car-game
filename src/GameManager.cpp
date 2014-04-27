
#include "GameManager.hpp"

#include <cassert>
#include <boost/math/constants/constants.hpp>

namespace car {

GameManager::GameManager(const Parameters& parameters, std::function<Track()> trackCreator) :
	parameters(parameters),
	track(trackCreator())
{
	track.check();
	init();
}

void GameManager::init() {
	model = Model{};
	model.setTrack(track);
	model.setCar(model.getTrack().createCar());
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
	rayCount = neuralNetwork.getInputNeuronCount() - extraNeuronCount;
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
	using namespace boost::math::float_constants;

	Weights inputs(rayCount + extraNeuronCount);

	const float wallDistanceDamping = 5.f;
	const float speedDamping = 5.f;
	const float checkpointAngleDamping = pi * 6.f;

	const sf::Vector2f& carPosition = model.getCar().getPosition();
	for (unsigned i = 0; i < rayCount; ++i) {
		auto rayPoint = rayPoints[i];
		if (rayPoint) {
			float distance = getDistance(carPosition, *rayPoint);
			inputs[i] = sigmoidApproximation(distance/wallDistanceDamping);
		} else {
			inputs[i] = 1.f;
		}
	}
	inputs[rayCount] = sigmoidApproximation(model.getCar().getSpeed()/speedDamping);
	inputs[rayCount+1] = sigmoidApproximation(model.getCheckpointAngle()/checkpointAngleDamping);

	return neuralNetwork.evaluateInput(inputs);
}

}

