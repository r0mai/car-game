#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "NeuronLayer.hpp"

namespace car {

class NeuralNetwork {
public:
	NeuralNetwork() = default;

	NeuralNetwork(
			unsigned hiddenLayerCount,
			unsigned hiddenLayerNeuronCount,
			unsigned inputNeuronCount,
			unsigned outputNeuronCount);

	static unsigned getWeightCountForNetwork(
			unsigned hiddenLayerCount,
			unsigned hiddenLayerNeuronCount,
			unsigned inputNeuronCount,
			unsigned outputNeuronCount);

	Weights getWeights() const;
	void setWeights(const Weights& weights);
	unsigned getWeightCount() const;

	unsigned getInputNeuronCount() const;
	unsigned getOutputNeuronCount() const;

	Weights evaluateInput(const Weights& input) const;

private:
	unsigned inputNeuronCount;

	std::vector<NeuronLayer> layers;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned version);
};

template<class Archive>
void NeuralNetwork::serialize(Archive& ar, const unsigned /*version*/) {
	ar & inputNeuronCount;
	ar & layers;
}

}

#endif /* !NEURALNETWORK_HPP */
