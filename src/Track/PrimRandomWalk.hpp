#ifndef SRC_TRACK_PRIMRANDOMWALK_HPP
#define SRC_TRACK_PRIMRANDOMWALK_HPP

#include "BaseRandomWalk.hpp"

namespace car { namespace track {

namespace randomWalk {

class PrimRandomWalk: public BaseRandomWalk {
public:
	virtual void initializeEdge(Graph& graph, const Edge& edge) override;
	virtual void randomWalk(Graph& graph, const Vertex& rootVertex)  override;
};

} // namespace randomWalk

}} /* namespace car::track */



#endif /* SRC_TRACK_PRIMRANDOMWALK_HPP */
