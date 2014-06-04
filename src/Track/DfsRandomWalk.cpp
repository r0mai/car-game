#include "DfsRandomWalk.hpp"
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>


namespace car { namespace track {

namespace randomWalk {

void DfsRandomWalk::initializeEdge(Graph& graph, const Edge& edge) {
	boost::random::uniform_real_distribution<float> distribution{0.f, 1.f};
	float weight = distribution(*rng);
	boost::put(boost::edge_weight, graph, edge, weight);
}

void DfsRandomWalk::randomWalk(Graph& graph, const Vertex& rootVertex) {
	std::vector<std::pair<Vertex, Vertex>> edges;

	for (const auto& edge: boost::iterator_range<EdgeIterator>{boost::edges(graph)}) {
		edges.emplace_back(boost::source(edge, graph), boost::target(edge, graph));
	}

	for (const auto& edge: edges) {
		boost::remove_edge(edge.first, edge.second, graph);
	}

	for (std::size_t i = 0; i < edges.size(); ++i) {
		boost::random::uniform_int_distribution<std::size_t> dist{i, edges.size() - 1};
		std::swap(edges[i], edges[dist(*rng)]);
	}

	for (const auto& edge: edges) {
		boost::add_edge(edge.first, edge.second, graph);
	}

	boost::depth_first_search(graph,
			boost::vertex_index_map(boost::get(boost::vertex_index, graph))
			.root_vertex(rootVertex)
			.color_map(boost::get(&VertexProperties::color, graph))
			.visitor(boost::make_dfs_visitor(
					boost::record_predecessors(
					boost::get(&VertexProperties::predecessor, graph), boost::on_tree_edge())))
		);
}

} // namespace randomWalk

}} /* namespace car::track */



