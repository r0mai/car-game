#ifndef SRC_TRACK_WILSONRANDOMWALK_HPP
#define SRC_TRACK_WILSONRANDOMWALK_HPP

#include "BaseRandomWalk.hpp"

namespace car { namespace track {

namespace randomWalk {

class WilsonRandomWalk: public BaseRandomWalk {
public:
	virtual void initializeEdge(Graph& graph, const Edge& edge) override;
	virtual void randomWalk(Graph& graph, const Vertex& rootVertex)  override;
};

} // namespace randomWalk

}} /* namespace car::track */


#endif /* SRC_TRACK_WILSONRANDOMWALK_HPP */
