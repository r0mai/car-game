
#include "LayeredNeuralNetwork.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>

#include "mathUtil.hpp"

namespace car {

Weights LayeredNeuralNetwork::getWeights() const {
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

void LayeredNeuralNetwork::setWeights(const Weights& weights) {
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

unsigned LayeredNeuralNetwork::getWeightCount() const {
	unsigned count = 0;
	for (const NeuronLayer& layer : layers) {
		for (const Neuron& neuron : layer.neurons) {
			count += neuron.weights.size();
		}
	}
	return count;
}

unsigned LayeredNeuralNetwork::getInputNeuronCount() const {
	return inputNeuronCount;
}

unsigned LayeredNeuralNetwork::getOutputNeuronCount() const {
	return layers.back().neurons.size();
}

unsigned LayeredNeuralNetwork::getHiddenLayerCount() const {
	return layers.size() - 1;
}

unsigned LayeredNeuralNetwork::getHiddenLayerNeuronCount() const {
	return layers.size() > 1 ? layers[0].neurons.size() : 0;
}

bool LayeredNeuralNetwork::isRecurrent() const {
	return static_cast<bool>(layers[0].neurons[0].recurrence);
}

const std::map<std::string, std::string>& LayeredNeuralNetwork::getExternalParameters() const {
	return externalParameters;
}

}

