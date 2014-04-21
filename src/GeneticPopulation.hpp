#ifndef GENETICPOPULATION_HPP
#define GENETICPOPULATION_HPP

#include <vector>

#include "NeuralNetwork.hpp"
#include "Genome.hpp"

namespace car {


//This class implements Genetic algorithms to mutate its population
class GeneticPopulation {
public:

	GeneticPopulation(unsigned populationSize, unsigned numberOfWeights);
private:
	typedef std::vector<Genome> Genomes;

	Genomes population;

	double mutationRate = 0.1;
	double crossoverRate = 0.7;

	unsigned generationCount = 0;
};

}

#endif
