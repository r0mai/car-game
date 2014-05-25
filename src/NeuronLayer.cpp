
#include "NeuronLayer.hpp"

namespace car {

NeuronLayer::NeuronLayer(unsigned neuronCount, unsigned inputCount, bool hasRecurrence) {
	neurons.reserve(neuronCount);
	for ( unsigned i = 0; i < neuronCount; ++i ) {
		neurons.push_back(Neuron(inputCount, hasRecurrence));
	}
}

}

