#ifndef NEURONLAYER_HPP
#define NEURONLAYER_HPP

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "Neuron.hpp"

namespace car {

struct NeuronLayer {
	NeuronLayer() = default;
	NeuronLayer(unsigned neuronCount, unsigned inputCount, bool hasRecurrence);

	std::vector<Neuron> neurons;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned version);
};

template<class Archive>
void NeuronLayer::serialize(Archive& ar, const unsigned /*version*/) {
	ar & neurons;
}

}

#endif /* !NEURONLAYER_HPP */
