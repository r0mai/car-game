
#include "GeneticPopulation.hpp"

#include "randomUtil.hpp"

namespace car {

GeneticPopulation::GeneticPopulation(unsigned populationSize, unsigned numberOfWeights) {
	for ( unsigned i = 0; i < populationSize; ++i ) {
		Weights weights(numberOfWeights);
		for ( Weight& weight : weights ) {
			weight = randomReal(-1.f, 1.f);
		}
		population.push_back(Genome(weights, 0.f));
	}
}

}

