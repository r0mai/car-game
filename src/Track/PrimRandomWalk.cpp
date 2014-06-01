#include "PrimRandomWalk.hpp"
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/random/uniform_real_distribution.hpp>


namespace car { namespace track {

namespace randomWalk {

void PrimRandomWalk::initializeEdge(Graph& graph, const Edge& edge) {
	boost::random::uniform_real_distribution<float> distribution{0.f, 1.f};
	float weight = distribution(*rng);
	boost::put(boost::edge_weight, graph, edge, weight);
}

void PrimRandomWalk::randomWalk(Graph& graph, const Vertex& rootVertex) {
	boost::prim_minimum_spanning_tree(graph,
			boost::get(&VertexProperties::predecessor, graph),
			boost::root_vertex(rootVertex)
			.weight_map(boost::get(boost::edge_weight, graph))
			.color_map(boost::get(&VertexProperties::color, graph))
		);
}

} // namespace randomWalk

}} /* namespace car::track */


