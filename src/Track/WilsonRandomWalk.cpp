#include "WilsonRandomWalk.hpp"
#include <boost/graph/random_spanning_tree.hpp>


namespace car { namespace track {

namespace randomWalk {

void WilsonRandomWalk::initializeEdge(Graph& graph, const Edge& edge) {
	boost::put(boost::edge_weight, graph, edge, 1.f);
}

void WilsonRandomWalk::randomWalk(Graph& graph, const Vertex& rootVertex) {
	boost::random_spanning_tree(graph, *rng,
			boost::vertex_index_map(boost::get(boost::vertex_index, graph))
			.root_vertex(rootVertex)
			.weight_map(boost::get(boost::edge_weight, graph))
			.predecessor_map(boost::get(&VertexProperties::predecessor, graph))
			.color_map(boost::get(&VertexProperties::color, graph))
		);
}

} // namespace randomWalk

}} /* namespace car::track */

