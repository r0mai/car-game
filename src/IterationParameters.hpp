#ifndef SRC_ITERATIONPARAMETERS_HPP
#define SRC_ITERATIONPARAMETERS_HPP

#include <boost/optional.hpp>
#include "MathExpression.hpp"

namespace car {

struct IterationParameters {
	boost::optional<unsigned> generationLimit;
	unsigned printoutFrequency = 1;
	MathExpression fitnessExpression = parseMathExpression(
		"0.5*td + (ccps > cps)*(100*cps + 2*(ccps-cps)) + (ccps <= cps)*(100*ccps)");
};

}

#endif /* SRC_ITERATIONPARAMETERS_HPP */
