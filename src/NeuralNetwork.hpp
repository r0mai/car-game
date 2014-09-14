#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <vector>
#include <map>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

#include "NeuronLayer.hpp"

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

	Weights getWeights() const;
	void setWeights(const Weights& weights);
	unsigned getWeightCount() const;

	unsigned getInputNeuronCount() const;
	unsigned getOutputNeuronCount() const;

	Weights evaluateInput(const Weights& input);

	std::string getExternalParameter(const std::string& key) const;
	void setExternalParameter(std::string key, std::string value);

private:
	unsigned inputNeuronCount;

	std::vector<NeuronLayer> layers;

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
	void serialize(Archive& ar, const unsigned version);
};

template<class Archive>
void NeuralNetwork::serialize(Archive& ar, const unsigned /*version*/) {
	ar & inputNeuronCount;
	ar & layers;
	ar & externalParameters;
}

NeuralNetwork loadNeuralNetworkFromFile(const std::string& fileName);

}

BOOST_CLASS_VERSION(car::NeuralNetwork, 0)

#endif /* !NEURALNETWORK_HPP */
