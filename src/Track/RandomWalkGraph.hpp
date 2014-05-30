#ifndef SRC_TRACK_RANDOMWALKGRAPH_HPP
#define SRC_TRACK_RANDOMWALKGRAPH_HPP

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/subgraph.hpp>

namespace car { namespace track {

namespace randomWalk {

struct VertexProperties {
	boost::default_color_type color = boost::white_color;
	std::size_t predecessor = 0;
};

using BaseGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
	  VertexProperties,
	  boost::property<boost::edge_index_t, std::size_t,
	  boost::property<boost::edge_weight_t, double>>>;
using Graph = boost::subgraph<BaseGraph>;
using Vertex = boost::graph_traits<BaseGraph>::vertex_descriptor;
using VertexIterator = boost::graph_traits<BaseGraph>::vertex_iterator;
using Edge = boost::graph_traits<BaseGraph>::edge_descriptor;

} // namespace randomWalk

}} /* namespace car::track */

#endif /* SRC_TRACK_RANDOMWALKGRAPH_HPP */
