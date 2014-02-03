#ifndef GENOME_HPP
#define GENOME_HPP

#include "NeuronWeights.hpp"

namespace car {

class Genome {
public:
	Genome(const Weights& weights, float fitness);
private:
	Weights weights;
	float fitness;
};

}

#endif /* !GENOME_HPP */
