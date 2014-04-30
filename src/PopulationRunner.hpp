#ifndef POPULATIONRUNNER_HPP_
#define POPULATIONRUNNER_HPP_

#include <functional>
#include <vector>
#include <string>
#include <boost/asio/io_service.hpp>
#include "Parameters.hpp"
#include "GeneticPopulation.hpp"
#include "Track.hpp"
#include "AIGameManager.hpp"
#include "NeuralNetwork.hpp"

namespace car {

class Genome;

class PopulationRunner {
public:
	PopulationRunner(const Parameters& parameters,
		std::vector<std::function<Track()>> trackCreators,
		boost::asio::io_service& ioService);

	PopulationRunner(const PopulationRunner&) = delete;
	PopulationRunner& operator=(const PopulationRunner&) = delete;
	PopulationRunner(PopulationRunner&&) = default;
	PopulationRunner& operator=(PopulationRunner&&) = default;

	void runIteration();

	float getBestFitness() const { return bestFitness; }
	const Genome* getBestGenome() const { return bestGenome; }
	const GeneticPopulation& getPopulation() const { return population; }
	GeneticPopulation& getPopulation() { return population; }
private:
	struct NeuralControllerData {
		NeuralNetwork network;
		std::vector<AIGameManager> managers;
	};

	boost::asio::io_service* ioService;

	GeneticPopulation population;
	std::vector<NeuralControllerData> controllerDatas;
	float bestFitness = 0.f;
	const Genome* bestGenome = nullptr;

	void runSimulation(Genome& genome, NeuralControllerData& data);
	void updateBestFitness();
};

} /* namespace car */

#endif /* POPULATIONRUNNER_HPP_ */
