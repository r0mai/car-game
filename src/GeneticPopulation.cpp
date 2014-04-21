
#include <cassert>
#include <algorithm>

#include "GeneticPopulation.hpp"

#include "randomUtil.hpp"

namespace car {

GeneticPopulation::GeneticPopulation(unsigned populationSize, unsigned numberOfWeights) {
	for (unsigned i = 0; i < populationSize; ++i) {
		Weights weights(numberOfWeights);
		for (Weight& weight : weights) {
			weight = randomReal(-1, 1);
		}
		population.push_back(Genome(weights));
	}
}

const GeneticPopulation::Genomes& GeneticPopulation::getPopulation() const {
   return population;
}

void GeneticPopulation::mutate(Weights& weights) const {
	for (Weight& weight : weights) {
		if (randomReal(0, 1) < mutationRate) {
			weight += (randomReal(-1, 1) * maxPerturbation);
		}
	}
}

Genome GeneticPopulation::pickRoulette() const {

	float slice = randomReal(0, totalFitness);

	float fitnessSoFar = 0;

	for (const Genome& genome : population) {
		fitnessSoFar += genome.fitness;
		if (genome.fitness >= slice) {
			return genome;
		}
	}
	//we shouldn't ever get here (only if rounding error occurs)
	return population.back();
}


void GeneticPopulation::calculateStats() {
	assert(population.size() > 0);

	auto minmax = std::minmax_element(population.begin(), population.end());
	worstFitnessIndex = minmax.first - population.begin();
	bestFitnessIndex = minmax.second - population.end();

	totalFitness = std::accumulate(population.begin(), population.end(), 0.f,
			[](float sum, const Genome& genome) { return sum + genome.fitness; });
}

}

