
#include "Neuron.hpp"

#include "randomUtil.hpp"

namespace car {

Neuron::Neuron(unsigned inputCount) : weights(inputCount+1) {
	for ( Weight& weight : weights ) {
		weight = randomReal(-1, 1);
	}
}

}

