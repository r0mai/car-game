#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <vector>

#include "NeuronLayer.hpp"

namespace car {

class NeuronNetwork {
public:
	NeuronNetwork(
			unsigned hiddenLayerCount,
			unsigned hiddenLayerNeuronCount,
		   	unsigned outputNeuronCount,
			unsigned inputNeuronCount);

	Weights getWeights() const;
	void setWeights(const Weights& weights);
	unsigned getWeightCount() const;

	Weights evaluateInput(const Weights& input) const;

private:
	unsigned inputNeuronCount;

	std::vector<NeuronLayer> layers;
};

}

#endif /* !NEURALNETWORK_HPP */
