#ifndef GENOME_HPP
#define GENOME_HPP

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "NeuronWeights.hpp"

namespace car {

class Genome {
public:
	Genome() = default;
	Genome(const Weights& weights, float fitness = 0.f);

	Weights weights;
	float fitness = 0.f;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned version);
};

typedef std::vector<Genome> Genomes;

template<class Archive>
void Genome::serialize(Archive& ar, const unsigned /*version*/) {
	ar & weights;
	//it makes no sense to serialize the fitness. It depends on
	//many parameters, and is only used to cache the calculation
}


bool operator<(const Genome& left, const Genome& right);

}

#endif /* !GENOME_HPP */
