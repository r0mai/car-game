
#include "Genome.hpp"

namespace car {

Genome::Genome(const Weights& weights, float fitness) : weights(weights), fitness(fitness) {}

bool operator<(const Genome& left, const Genome& right) {
	return left.fitness < right.fitness;
}

}


