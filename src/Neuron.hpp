#ifndef NEURON_HPP
#define NEURON_HPP

#include "NeuronWeights.hpp"

namespace car {

class Neuron {
public:
	Neuron(unsigned inputCount);

	//size is 1+inputCount (bias)
	Weights weights;
};

}

#endif /* !NEURON_HPP */
