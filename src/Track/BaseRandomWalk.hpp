#ifndef SRC_TRACK_IRANDOMWALK_HPP
#define SRC_TRACK_IRANDOMWALK_HPP

#include "RandomWalkGraph.hpp"
#include "RandomGenerator.hpp"

namespace car { namespace track {

namespace randomWalk {

class BaseRandomWalk {
public:
	virtual void initializeEdge(Graph& graph, const Edge& edge) = 0;
	virtual void randomWalk(Graph& graph, const Vertex& rootVertex) = 0;
	void setRandomGenerator(RandomGenerator& rng) {
		this->rng = &rng;
	}
protected:
	RandomGenerator* rng;
};

} // namespace randomWalk

}} /* namespace car::track */


#endif /* SRC_TRACK_IRANDOMWALK_HPP */
