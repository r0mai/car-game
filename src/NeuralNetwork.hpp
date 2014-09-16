#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <vector>
#include <map>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

#include "NeuronWeights.hpp"
#include "Legacy/LayeredNeuralNetwork.hpp"

namespace car {

class NeuralNetwork {
public:
	NeuralNetwork() = default;

	NeuralNetwork(
			unsigned hiddenLayerCount,
			unsigned hiddenLayerNeuronCount,
			unsigned inputNeuronCount,
			unsigned outputNeuronCount,
			bool useRecurrence);

	static unsigned getWeightCountForNetwork(
			unsigned hiddenLayerCount,
			unsigned hiddenLayerNeuronCount,
			unsigned inputNeuronCount,
			unsigned outputNeuronCount,
			bool useRecurrence);

	const Weights& getWeights() const { return weights; }
	void setWeights(Weights weights) { this->weights = std::move(weights); }
	unsigned getWeightCount() const { return weights.size(); }

	unsigned getInputNeuronCount() const { return inputNeuronCount; }
	unsigned getOutputNeuronCount() const { return outputNeuronCount; }

	Weights evaluateInput(Weights input);

	std::string getExternalParameter(const std::string& key) const;
	void setExternalParameter(std::string key, std::string value);

private:
	unsigned hiddenLayerCount;
	unsigned hiddenLayerNeuronCount;
	unsigned inputNeuronCount;
	unsigned outputNeuronCount;
	bool useRecurrence;

	Weights weights;
	std::vector<float> recurrence;

	/*
	 * We want to achive the maximum flexibility here. Since this is only read
	 * when the neural network is loaded, efficiency is not a concern here.  A
	 * map is used because the neural network doesn't care about what the
	 * actual parameters are, it only stores values to be used by the front
	 * end. String is used as the key to increase readability of the front end
	 * code.
	 */
	std::map<std::string, std::string> externalParameters;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void load(Archive& ar, const unsigned version) {
		if (version < 1) {
			LayeredNeuralNetwork network;
			ar >> network.inputNeuronCount;
			ar >> network.layers;
			ar >> network.externalParameters;

			hiddenLayerCount = network.getHiddenLayerCount();
			hiddenLayerNeuronCount = network.getHiddenLayerNeuronCount();
			inputNeuronCount = network.getInputNeuronCount();
			outputNeuronCount = network.getOutputNeuronCount();
			useRecurrence = network.isRecurrent();
			weights = network.getWeights();
			externalParameters = network.getExternalParameters();
		} else {
			ar >> hiddenLayerCount;
			ar >> hiddenLayerNeuronCount;
			ar >> inputNeuronCount;
			ar >> outputNeuronCount;
			ar >> useRecurrence;
			ar >> recurrence;
			ar >> weights;
			ar >> externalParameters;
		}
	}

	template<class Archive>
	void save(Archive& ar, const unsigned /*version*/) const {
		ar << hiddenLayerCount;
		ar << hiddenLayerNeuronCount;
		ar << inputNeuronCount;
		ar << outputNeuronCount;
		ar << useRecurrence;
		ar << recurrence;
		ar << weights;
		ar << externalParameters;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER();

};

NeuralNetwork loadNeuralNetworkFromFile(const std::string& fileName);

}

BOOST_CLASS_VERSION(car::NeuralNetwork, 1)

#endif /* !NEURALNETWORK_HPP */
