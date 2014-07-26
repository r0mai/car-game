
#include "GameManager.hpp"

#include <cassert>
#include <boost/math/constants/constants.hpp>
#include "mathUtil.hpp"
#include "Line2.hpp"

namespace car {

GameManager::GameManager(const CommonParameters& parameters, track::TrackCreator trackCreator) :
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
	rayCount = neuralNetwork.getInputNeuronCount() - parameters.extraInputNeuronCount;
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

	Weights inputs(rayCount + parameters.extraInputNeuronCount);

	const float wallDistanceDamping = 5.f;
	const float speedDamping = 5.f;
	const float checkpointDirectionDamping = 0.2f;

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

	auto checkpointDirection = model.getCheckpointDirection();
	inputs[rayCount+1] = sigmoidApproximation(checkpointDirection.x/checkpointDirectionDamping);
	inputs[rayCount+2] = sigmoidApproximation(checkpointDirection.y/checkpointDirectionDamping);
	return neuralNetwork.evaluateInput(inputs);
}

}

