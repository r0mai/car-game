
#ifndef SRC_LEARNINGCONTROLLER_HPP
#define SRC_LEARNINGCONTROLLER_HPP

#include <functional>
#include <Track/TrackCreator.hpp>
#include "LearningParameters.hpp"

namespace boost { namespace asio {

class io_service;

}}


namespace car {

class GeneticPopulation;
class Genome;

class LearningController {
public:
	LearningController(const LearningParameters& parameters,
			const track::TrackCreators& trackCreators,
			boost::asio::io_service& ioService);
	void run();

private:
	void loadPopulation(GeneticPopulation& population) const;
	void savePopulation(const GeneticPopulation& population) const;

	boost::asio::io_service& ioService;
	LearningParameters parameters;
	track::TrackCreators trackCreators;

	void saveNeuralNetwork(const Genome& genome);
};

}

#endif /* SRC_LEARNINGCONTROLLER_HPP */

