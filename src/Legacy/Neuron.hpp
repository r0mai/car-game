#ifndef NEURON_HPP
#define NEURON_HPP

#include <boost/optional.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>

#include "NeuronWeights.hpp"

namespace car {

class Neuron {
public:
	Neuron() = default;

	//size is 1+inputCount (bias)
	Weights weights;
	boost::optional<Weight> recurrence;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned version);
};

template<class Archive>
void Neuron::serialize(Archive& ar, const unsigned /*version*/) {
	ar & weights;
	ar & recurrence;
}

}

#endif /* !NEURON_HPP */
