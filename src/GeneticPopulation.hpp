#ifndef GENETICPOPULATION_HPP
#define GENETICPOPULATION_HPP

#include <vector>

#include "NeuralNetwork.hpp"
#include "Genome.hpp"

namespace car {


//This class implements Genetic algorithms to mutate its population
class GeneticPopulation {
public:
	typedef std::vector<Genome> Genomes;

	GeneticPopulation(unsigned populationSize, unsigned numberOfWeights);

	const Genomes& getPopulation() const;

private:
	void mutate(Weights& weights) const;
	Genome pickRoulette() const;
	void calculateStats();

	Genomes population;

	unsigned generationCount = 0;

	unsigned bestFitnessIndex; //updated by calculateStats()
	unsigned worstFitnessIndex; //updated by calculateStats()
	float totalFitness; //updated by calculateStats()

	//constants
	float mutationRate = 0.1;
	float crossoverRate = 0.7;
	float maxPerturbation = 0.3;

};

}

#endif
