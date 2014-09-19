#ifndef GENOME_HPP
#define GENOME_HPP

#include <vector>
#include <memory>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "NeuronWeights.hpp"

namespace car {

class Genome {
public:
	Genome() = default;
	Genome(std::shared_ptr<const Weights> weights, float fitness = 0.f) :
		weights(std::move(weights)), fitness(fitness) {}

	std::shared_ptr<const Weights> weights;
	float fitness = 0.f;
	std::string debugInfo;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void load(Archive& ar, const unsigned /*version*/) {
		weights = loadWeights(ar);
	}

	template<class Archive>
	void save(Archive& ar, const unsigned /*version*/) const {
		ar << *weights;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER();
};

typedef std::vector<Genome> Genomes;

inline
bool operator<(const Genome& left, const Genome& right) {
	return left.fitness < right.fitness;
}

}

BOOST_CLASS_VERSION(car::Genome, 0)

#endif /* !GENOME_HPP */
