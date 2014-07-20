
#ifndef NEURALCONTROLLER_HPP_
#define NEURALCONTROLLER_HPP_

#include <functional>

#include "Parameters.hpp"
#include "Track/Track.hpp"
#include "Track/TrackCreator.hpp"
#include "boost/asio/io_service.hpp"

namespace car {

class GeneticPopulation;
class Genome;

class NeuralController {
public:
	NeuralController(const Parameters& parameters,
			track::TrackCreators trackCreators,
			boost::asio::io_service& ioService);
	void run();

private:
	void loadPopulation(GeneticPopulation& population) const;
	void savePopulation(const GeneticPopulation& population) const;

	boost::asio::io_service& ioService;
	Parameters parameters;
	track::TrackCreators trackCreators;

	void saveNeuralNetwork(const Genome& genome);
};

}

#endif

