#ifndef SRC_LEGACY_LAYEREDNEURALNETWORK_HPP
#define SRC_LEGACY_LAYEREDNEURALNETWORK_HPP

#include <vector>
#include <map>

#include "NeuronLayer.hpp"

namespace car {

class LayeredNeuralNetwork {
public:
	LayeredNeuralNetwork() = default;

	Weights getWeights() const;
	void setWeights(const Weights& weights);
	unsigned getWeightCount() const;

	unsigned getInputNeuronCount() const;
	unsigned getOutputNeuronCount() const;
	unsigned getHiddenLayerCount() const;
	unsigned getHiddenLayerNeuronCount() const;
	bool isRecurrent() const;

	const std::map<std::string, std::string>& getExternalParameters() const;

	unsigned inputNeuronCount;
	std::vector<NeuronLayer> layers;
	std::map<std::string, std::string> externalParameters;

};

}

#endif /* SRC_LEGACY_LAYEREDNEURALNETWORK_HPP */
