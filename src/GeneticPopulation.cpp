
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

const Genomes& GeneticPopulation::getPopulation() const {
	return population;
}

Genomes& GeneticPopulation::getPopulation() {
	return population;
}

void GeneticPopulation::evolve() {

	std::sort(population.begin(), population.end());

	calculateStats();
	Genomes newPopulation;

	assert((bestTopN * bestCopies) % 2 == 0);
	assert(population.size() % 2 == 0);

	pickBest(bestTopN, bestCopies, newPopulation);

	while (newPopulation.size() < population.size()) {
		Genome parent1 = pickRoulette();
		Genome parent2 = pickRoulette();

		Weights child1, child2;

		crossover(parent1.weights, parent2.weights, child1, child2);

		mutate(child1);
		mutate(child2);

		newPopulation.push_back(Genome(child1, 0));
		newPopulation.push_back(Genome(child2, 0));
	}

	population = newPopulation;
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

void GeneticPopulation::crossover(
	const Weights& parent1,
	const Weights& parent2,
	Weights& child1,
	Weights& child2) const {

	assert(parent1.size() == parent2.size());

	if (randomReal(0, 1) > crossoverRate || parent1 == parent2) {
		child1 = parent1;
		child2 = parent2;
		return;
	}

	unsigned crossoverPoint = static_cast<unsigned>(randomInt(0, parent1.size()));

	child1.clear();
	child2.clear();

	//create the offspring
	for (unsigned i = 0; i < crossoverPoint; ++i) {
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
	}

	for (unsigned i = crossoverPoint; i < parent1.size(); ++i) {
		child1.push_back(parent2[i]);
		child2.push_back(parent1[i]);
	}
}

void GeneticPopulation::pickBest(unsigned topN, unsigned copies, Genomes& newPopulation) {
	for (unsigned i = 0; i < topN; ++i) {
		for (unsigned j = 0; j < copies; ++j) {
			newPopulation.push_back(population[population.size() - 1 - i]);
		}
	}
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

