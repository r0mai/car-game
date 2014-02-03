#ifndef NEURONLAYER_HPP
#define NEURONLAYER_HPP

#include <vector>

#include "Neuron.hpp"

namespace car {

struct NeuronLayer {
	NeuronLayer(unsigned neuronCount, unsigned inputCount);

	std::vector<Neuron> neurons;
};

}

#endif /* !NEURONLAYER_HPP */
