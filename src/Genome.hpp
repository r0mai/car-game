#ifndef GENOME_HPP
#define GENOME_HPP

#include "NeuronWeights.hpp"

namespace car {

class Genome {
public:
	Genome() = default;
	Genome(const Weights& weights, float fitness = 0.f);

	Weights weights;
	float fitness = 0.f;
};

bool operator<(const Genome& left, const Genome& right);

}

#endif /* !GENOME_HPP */
