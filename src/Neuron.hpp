#ifndef NEURON_HPP
#define NEURON_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "NeuronWeights.hpp"

namespace car {

class Neuron {
public:
	Neuron() = default;
	Neuron(unsigned inputCount);

	//size is 1+inputCount (bias)
	Weights weights;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned version);
};

template<class Archive>
void Neuron::serialize(Archive& ar, const unsigned /*version*/) {
	ar & weights;
}

}

#endif /* !NEURON_HPP */
