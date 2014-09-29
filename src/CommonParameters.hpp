#ifndef SRC_COMMONPARAMETERS_HPP
#define SRC_COMMONPARAMETERS_HPP

#include <vector>
#include <string>
#include "CarParameters.hpp"
#include "PerturbationParameters.hpp"

namespace car {

struct CommonParameters {

	CarParameters carParameters;
	PerturbationParameters perturbationParameters;

	unsigned rayCount = 14;
	unsigned checkpointLookAhead = 1;
	unsigned physicsTimeStepsPerSecond = 64;

	// for speed
	constexpr static unsigned extraGlobalInputNeuronCount() { return  1; }
	// 2 for orientation, 2*(2+1) for left/right edge orientation/distance
	constexpr static unsigned inputNeuronCountPerCheckpoint() { return  8; }
	constexpr static unsigned outputNeuronCount() { return 3; }

	unsigned getInputNeuronCount() const {
		return rayCount + extraGlobalInputNeuronCount() + checkpointLookAhead * inputNeuronCountPerCheckpoint();
	}

};

inline
float getPhysicsTimeStep(const CommonParameters& parameters) {
	return 1.f/parameters.physicsTimeStepsPerSecond;
}

class NeuralNetwork;

void setNeuralNetworkExternalParameters(const CommonParameters& parameters, NeuralNetwork& neuralNetwork);
void getNeuralNetworkExternalParameters(CommonParameters& parameters, const NeuralNetwork& neuralNetwork);

}


#endif /* SRC_COMMONPARAMETERS_HPP */
