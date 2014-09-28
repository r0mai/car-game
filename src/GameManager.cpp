
#include "GameManager.hpp"

#include <cassert>
#include <boost/math/constants/constants.hpp>
#include "mathUtil.hpp"
#include "Line2.hpp"
#include "PerturbationParameters.hpp"

namespace car {

GameManager::GameManager(const CommonParameters& parameters,
		std::shared_ptr<const track::Track> track) :
	parameters(parameters),
	physicsTimeStep(getPhysicsTimeStep(parameters)),
	model{parameters.carParameters},
	track(std::move(track))
{
	init();
}

void GameManager::init() {
	model = Model{parameters.carParameters};
	model.setTrack(track);
	model.setCar(model.getTrack()->createCar(parameters.carParameters, PerturbationParameters()));
	rayPoints = model.getRayPoints(parameters.rayCount);
}

void GameManager::advance() {
	if (isAIControl) {
		controlCar();
	}
	model.advanceTime(physicsTimeStep);
	rayPoints = model.getRayPoints(parameters.rayCount);
}

void GameManager::setNeuralNetwork(const NeuralNetwork& network) {
	assert(network.getOutputNeuronCount() == 3);
	assert(network.getInputNeuronCount() > 0);

	neuralNetwork = network;
	getNeuralNetworkExternalParameters(parameters, neuralNetwork);
}

void GameManager::controlCar() {
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

Weights GameManager::callNeuralNetwork() {
	using namespace boost::math::float_constants;

//	std::cerr << "neu " << neuralNetwork.getInputNeuronCount() << " param " << parameters.getInputNeuronCount() << " (" << parameters.checkpointLookAhead << ")" << std::endl;
	Weights inputs(neuralNetwork.getInputNeuronCount());

	const float wallDistanceDamping = 5.f;
	const float speedDamping = 5.f;
	const float checkpointDirectionDamping = 0.2f;

	const sf::Vector2f& carPosition = model.getCar().getPosition();
	for (unsigned i = 0; i < parameters.rayCount; ++i) {
		auto rayPoint = rayPoints[i];
		if (rayPoint) {
			float distance = getDistance(carPosition, *rayPoint);
			inputs[i] = sigmoidApproximation(distance/wallDistanceDamping);
		} else {
			inputs[i] = 1.f;
		}
	}
	inputs[parameters.rayCount] = sigmoidApproximation(model.getCar().getSpeed()/speedDamping);

	int inputId = parameters.rayCount;

	auto checkpointInformations = model.getCheckpointInformation(parameters.checkpointLookAhead);
	for (const auto& checkpointInformation: checkpointInformations) {
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.orientation.x/checkpointDirectionDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.orientation.y/checkpointDirectionDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.leftEdgeDistance/wallDistanceDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.leftEdgeOrientation.x/checkpointDirectionDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.leftEdgeOrientation.y/checkpointDirectionDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.rightEdgeDistance/wallDistanceDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.rightEdgeOrientation.x/checkpointDirectionDamping);
		inputs[++inputId] = sigmoidApproximation(checkpointInformation.rightEdgeOrientation.y/checkpointDirectionDamping);
	}
	return neuralNetwork.evaluateInput(inputs);
}

}

