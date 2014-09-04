
#include "NeuralNetwork.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>

#include "mathUtil.hpp"

namespace car {

NeuralNetwork::NeuralNetwork(
		unsigned hiddenLayerCount,
		unsigned hiddenLayerNeuronCount,
		unsigned inputNeuronCount,
		unsigned outputNeuronCount,
		bool useRecurrence) : inputNeuronCount(inputNeuronCount)
{
	if (hiddenLayerCount > 0) {
		layers.push_back(NeuronLayer(hiddenLayerNeuronCount, inputNeuronCount, useRecurrence));
		for (unsigned i = 0; i < hiddenLayerCount - 1; ++i) {
			layers.push_back(NeuronLayer(hiddenLayerNeuronCount, hiddenLayerNeuronCount, useRecurrence));
		}
		layers.push_back(NeuronLayer(outputNeuronCount, hiddenLayerNeuronCount, useRecurrence));
	} else {
		layers.push_back(NeuronLayer(outputNeuronCount, inputNeuronCount, useRecurrence));
	}
}

unsigned NeuralNetwork::getWeightCountForNetwork(
		unsigned hiddenLayerCount,
		unsigned hiddenLayerNeuronCount,
		unsigned inputNeuronCount,
		unsigned outputNeuronCount,
		bool useRecurrence)
{
	//we shouldn't create an object here, but this is quicker for now
	return NeuralNetwork(hiddenLayerCount, hiddenLayerNeuronCount, inputNeuronCount, outputNeuronCount, useRecurrence).getWeightCount();
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

unsigned NeuralNetwork::getInputNeuronCount() const {
	return inputNeuronCount;
}

unsigned NeuralNetwork::getOutputNeuronCount() const {
	return layers.back().neurons.size();
}

Weights NeuralNetwork::evaluateInput(const Weights& input) {
	assert(input.size() == inputNeuronCount);

	Weights nextInput = input;
	Weights output;
	for (NeuronLayer& layer : layers) {
		output.clear();
		for (Neuron& neuron : layer.neurons) {
			output.push_back(neuron.run(nextInput));
		}
		nextInput = output; //we could move here probably
	}
	return output;
}

NeuralNetwork loadNeuralNetworkFromFile(const std::string& fileName) {
	NeuralNetwork result;

	std::ifstream ifs(fileName);
	boost::archive::text_iarchive ia(ifs);
	ia >> result;

	return result;
}

std::string NeuralNetwork::getExternalParameter(const std::string& key) const {
	auto it = externalParameters.find(key);
	//std::cerr << "get " << key << " -> " << (it == externalParameters.end() ? "" : it->second) << std::endl;
	return it == externalParameters.end() ? "" : it->second;
}

void NeuralNetwork::setExternalParameter(std::string key, std::string value) {
	//std::cerr << "set " << key << " -> " << value << std::endl;
	externalParameters[std::move(key)] = std::move(value);
}

}

