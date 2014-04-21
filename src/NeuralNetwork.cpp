
#include "NeuralNetwork.hpp"

#include <cassert>
#include <cmath>

#include "mathUtil.hpp"

namespace car {

NeuralNetwork::NeuralNetwork(
		unsigned hiddenLayerCount,
		unsigned hiddenLayerNeuronCount,
	   	unsigned outputNeuronCount,
	   	unsigned inputNeuronCount) : inputNeuronCount(inputNeuronCount)
{
	if (hiddenLayerCount > 0) {
		layers.push_back(NeuronLayer(hiddenLayerNeuronCount, inputNeuronCount));
		for (unsigned i = 0; i < hiddenLayerCount - 1; ++i) {
			layers.push_back(NeuronLayer(hiddenLayerNeuronCount, hiddenLayerNeuronCount));
		}
		layers.push_back(NeuronLayer(outputNeuronCount, hiddenLayerNeuronCount));
	} else {
		layers.push_back(NeuronLayer(outputNeuronCount, inputNeuronCount));
	}
}

Weights NeuralNetwork::getWeights() const {
	Weights result;
	for (const NeuronLayer& layer : layers) {
		for (const Neuron& neuron : layer.neurons) {
			for (Weight weight : neuron.weights) {
				result.push_back(weight);
			}
		}
	}
	return result;
}

void NeuralNetwork::setWeights(const Weights& weights) {
	assert(weights.size() == getWeightCount());
	unsigned weightIndex = 0;
	for (NeuronLayer& layer : layers) {
		for (Neuron& neuron : layer.neurons) {
			for (Weight& weight : neuron.weights) {
				weight = weights[weightIndex++];
			}
		}
	}
}

unsigned NeuralNetwork::getWeightCount() const {
	unsigned count = 0;
	for (const NeuronLayer& layer : layers) {
		for (const Neuron& neuron : layer.neurons) {
			count += neuron.weights.size();
		}
	}
	return count;
}

Weights NeuralNetwork::evaluateInput(const Weights& input) const {
	assert(input.size() == inputNeuronCount);

	Weights nextInput = input;
	Weights output;
	for (const NeuronLayer& layer : layers) {
		output.clear();
		for (const Neuron& neuron : layer.neurons) {
			Weight netInput = 0;
			assert(neuron.weights.size() - 1 == nextInput.size());
			for (unsigned i = 0; i < nextInput.size(); ++i) {
				netInput += neuron.weights[i]*nextInput[i];
			}
			netInput += -1.f*neuron.weights.back();
			//sigmoid like function : x/(1+abs(x))
			output.push_back(sigmoidApproximation(netInput));
		}
		nextInput = output; //we could move here probably
	}
	return output;
}

}

