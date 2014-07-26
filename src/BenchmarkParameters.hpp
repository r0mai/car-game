#ifndef SRC_BENCHMARKPARAMETERS_HPP
#define SRC_BENCHMARKPARAMETERS_HPP

#include "CommonParameters.hpp"
#include "CarInputParameters.hpp"
#include "IterationParameters.hpp"

namespace car {

struct BenchmarkParameters {
	CommonParameters commonParameters;
	IterationParameters iterationParameters;
	CarInputParameters carInputParameters;
};

}


#endif /* SRC_BENCHMARKPARAMETERS_HPP */
