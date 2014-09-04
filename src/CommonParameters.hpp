#ifndef SRC_COMMONPARAMETERS_HPP
#define SRC_COMMONPARAMETERS_HPP

#include <vector>
#include <string>

namespace car {

struct CommonParameters {

	unsigned rayCount = 14;
	unsigned physicsTimeStepsPerSecond = 64;

	// for speed
	const unsigned extraGlobalInputNeuronCount = 1;
	// 2 for orientation, 2*(2+1) for left/right edge orientation/distance
	const unsigned inputNeuronCountPerCheckpoint = 8;
	const unsigned extraInputNeuronCount = extraGlobalInputNeuronCount + inputNeuronCountPerCheckpoint;
	const unsigned outputNeuronCount = 3;

	unsigned getInputNeuronCount() const {
		return rayCount + extraInputNeuronCount;
	}

	// No default copy because of const parameters

	CommonParameters() = default;

	CommonParameters(const CommonParameters& other):
		rayCount(other.rayCount),
		physicsTimeStepsPerSecond(other.physicsTimeStepsPerSecond)
	{ }

	CommonParameters& operator=(const CommonParameters& other) {
		rayCount = other.rayCount;
		physicsTimeStepsPerSecond = other.physicsTimeStepsPerSecond;
		return *this;
	}

};

inline
float getPhysicsTimeStep(const CommonParameters& parameters) {
	return 1.f/parameters.physicsTimeStepsPerSecond;
}

}


#endif /* SRC_COMMONPARAMETERS_HPP */
