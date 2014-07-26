#ifndef SRC_COMMONPARAMETERS_HPP
#define SRC_COMMONPARAMETERS_HPP

#include <vector>
#include <string>

namespace car {

struct CommonParameters {

	unsigned rayCount = 14;
	unsigned physicsTimeStepsPerSecond = 64;

	//1 for speed input and 2 for the direction of the next CP
	const unsigned extraInputNeuronCount = 3;
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

}


#endif /* SRC_COMMONPARAMETERS_HPP */
