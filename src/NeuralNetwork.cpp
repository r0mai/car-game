
#include "NeuralNetwork.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>

#include "mathUtil.hpp"
#include "randomUtil.hpp"

namespace car {

NeuralNetwork::NeuralNetwork(
		unsigned hiddenLayerCount,
		unsigned hiddenLayerNeuronCount,
		unsigned inputNeuronCount,
		unsigned outputNeuronCount,
		bool useRecurrence) :
			hiddenLayerCount(hiddenLayerCount),
			hiddenLayerNeuronCount(hiddenLayerNeuronCount),
			inputNeuronCount(inputNeuronCount),
			outputNeuronCount(outputNeuronCount),
			useRecurrence(useRecurrence),
			weights((hiddenLayerCount == 0) ?
				(outputNeuronCount * (inputNeuronCount + 1 + useRecurrence)) :
				(hiddenLayerNeuronCount * (inputNeuronCount + outputNeuronCount +
						(hiddenLayerCount - 1) * hiddenLayerNeuronCount) +
					(hiddenLayerCount * hiddenLayerNeuronCount + outputNeuronCount) * (
						1 + useRecurrence))
				),
			recurrence(outputNeuronCount + hiddenLayerCount * hiddenLayerNeuronCount)
{
	for (Weight& weight : weights) {
		weight = randomReal(-1, 1);
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

Weights NeuralNetwork::evaluateInput(Weights input) {
	assert(input.size() == inputNeuronCount);

	Weights output;

	unsigned weightIndex = 0;
	for (unsigned layer = 0; layer <= hiddenLayerCount; ++layer) {
		unsigned neuronCount = (layer == hiddenLayerCount) ?
				outputNeuronCount :
				hiddenLayerNeuronCount;
		unsigned weightCountPerNeuron = (layer == 0) ?
				inputNeuronCount :
				hiddenLayerNeuronCount;

		output.clear();
		output.reserve(neuronCount);
		for (unsigned neuron = 0; neuron < neuronCount; ++neuron) {
			assert(weightCountPerNeuron == input.size());

			Weight netInput = 0;

			for (auto value: input) {
				netInput += weights[weightIndex++]*value;
			}
			int recurrenceIndex = layer * hiddenLayerNeuronCount + inputNeuronCount;
			if (useRecurrence) {
				netInput += recurrence[recurrenceIndex] * weights[weightIndex++];
			}
			netInput += -1.f * weights[weightIndex++];

			Weight sigmoid = sigmoidApproximation(netInput);
			if (useRecurrence) {
				recurrence[recurrenceIndex] = sigmoid;
			}
			output.push_back(sigmoid);

		}
		input = output; //we could move here probably
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
	return it == externalParameters.end() ? "" : it->second;
}

void NeuralNetwork::setExternalParameter(std::string key, std::string value) {
	externalParameters[std::move(key)] = std::move(value);
}

}

