#ifndef SRC_PERTURBATIONPARAMETERS_HPP
#define SRC_PERTURBATIONPARAMETERS_HPP

#include "CarParameters.hpp"

namespace car {

struct PerturbationParameters: CarParameters {
	int seed = 0;

	PerturbationParameters() {
		cDrag = 0;
		cRollingResistance = 0;
		pEngine = 0;
		fEngineMax = 0;
		fBrake = 0;
		mass = 0;
		maxTurnAngle = 0;
		turnRate = 0;
		rearCMDistance = 0;
		frontCMDistance = 0;
		carWidth = 0;
		throttleIncreaseSpeed = 0;
		throttleDecreaseSpeed = 0;
		brakeIncreaseSpeed = 0;
		brakeDecreaseSpeed = 0;
		turnSpeed = 0;
	}
};

}


#endif /* SRC_PERTURBATIONPARAMETERS_HPP */
