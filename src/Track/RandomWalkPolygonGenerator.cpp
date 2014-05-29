#include "RandomWalkPolygonGenerator.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/random_spanning_tree.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "MatrixAdaptor.hpp"

namespace car { namespace track {

namespace {

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

template <typename Filter>
Graph& createSubgraph(Graph& graph, const Filter& filter) {
	auto& subgraph = graph.create_subgraph();

	for (const auto& vertex: boost::iterator_range<VertexIterator>{
			boost::vertices(graph)}) {
		if (filter(graph, vertex)) {
			boost::add_vertex(vertex, subgraph);
		}
	}

	return subgraph;
}

template <typename Filter>
class MatrixFilter {
public:
	MatrixFilter(const MatrixAdaptor& matrix, const Filter& filter):
		matrix(matrix), filter(filter)
	{}

	template <typename Graph, typename Vertex>
	bool operator()(const Graph& graph, const Vertex& vertex) const {
		std::size_t index = boost::get(boost::vertex_index, graph, vertex);
		auto coordinate = matrix.coordinateFromPosition(index);
		return filter(coordinate);
	}
private:
	MatrixAdaptor matrix;
	Filter filter;
};

template <typename Filter>
MatrixFilter<Filter> matrixFilter(const MatrixAdaptor& matrix, const Filter& filter) {
	return MatrixFilter<Filter>{matrix, filter};
}

template <typename Graph, class RandomGenerator>
void randomWalk(Graph& subgraph, const MatrixAdaptor& matrix, RandomGenerator& rng,
		const MatrixCoordinate& beginCoordinate, const MatrixCoordinate& endCoordinate,
		std::vector<MatrixCoordinate>& resultCoordinates) {
	boost::random_spanning_tree(subgraph, rng,
			boost::vertex_index_map(boost::get(boost::vertex_index, subgraph))
			.root_vertex(subgraph.global_to_local(
					matrix.positionFromCoordinate(endCoordinate)))
			.weight_map(boost::get(boost::edge_weight, subgraph))
			.predecessor_map(boost::get(&VertexProperties::predecessor, subgraph))
			.color_map(boost::get(&VertexProperties::color, subgraph))
		);

	for (Vertex position =
				subgraph.global_to_local(matrix.positionFromCoordinate(beginCoordinate));
			subgraph.local_to_global(position) !=
				matrix.positionFromCoordinate(endCoordinate);
			position = subgraph[position].predecessor) {
		assert(position != boost::graph_traits<Graph>::null_vertex());
		auto realPosition = subgraph.local_to_global(position);
		auto coordinate = matrix.coordinateFromPosition(realPosition);
		resultCoordinates.push_back(coordinate);
	}

}

}

std::vector<sf::Vector2f> RandomWalkPolygonGenerator::operator()(
		RandomGenerator& rng) const {
	MatrixAdaptor matrix{params.horizontalResolution, params.verticalResolution};

	std::vector<Edge> edges;
	Graph mainGraph{matrix.size()};

	std::size_t edgeIndex = 0;
	for (const auto& vertex: boost::iterator_range<VertexIterator>{
			boost::vertices(mainGraph)}) {
		auto coordinate = matrix.coordinateFromPosition(vertex);
		auto addEdge = [&](int dx, int dy) {
				MatrixCoordinate neighbour{coordinate.x + dx, coordinate.y + dy};
				auto neighbourPosition = matrix.positionFromCoordinate(neighbour);
				if (neighbourPosition != MatrixAdaptor::outsideRange()) {
					auto edge = boost::add_edge(vertex, neighbourPosition, mainGraph).first;
					boost::put(boost::edge_index, mainGraph, edge, edgeIndex++);
					boost::put(boost::edge_weight, mainGraph, edge, 1.f);
				}
			};

		addEdge(1, 0);
		addEdge(0, 1);
	}

	auto diagonalEnd = std::min(params.horizontalResolution, params.verticalResolution) - 1;
	MatrixCoordinate beginCoordinate{0, 0};
	MatrixCoordinate endCoordinate{diagonalEnd, diagonalEnd};
	auto& subgraph1 = createSubgraph(mainGraph,
		matrixFilter(matrix, [&](const MatrixCoordinate& coordinate) {
			return coordinate == beginCoordinate || coordinate == endCoordinate ||
					coordinate.x < coordinate.y;
		}));
	auto& subgraph2 = createSubgraph(mainGraph,
		matrixFilter(matrix, [&](const MatrixCoordinate& coordinate) {
			return coordinate == beginCoordinate || coordinate == endCoordinate ||
					coordinate.x >= coordinate.y;
		}));

	std::vector<MatrixCoordinate> resultCoordinates;

	randomWalk(subgraph1, matrix, rng, beginCoordinate, endCoordinate, resultCoordinates);
	randomWalk(subgraph2, matrix, rng, endCoordinate, beginCoordinate, resultCoordinates);

	std::vector<sf::Vector2f> result;
	result.reserve(resultCoordinates.size());
	const float originX = -0.5f * params.horizontalResolution * params.gridSize;
	const float originY = -0.5f * params.verticalResolution * params.gridSize;
	boost::random::uniform_real_distribution<float> jitterDistribution{
			  -params.jitter, params.jitter};
	std::cerr << "horizontal resolution = " << params.horizontalResolution <<
		", vertical resolution = " << params.verticalResolution <<
		", grid size = " << params.gridSize << ", jitter = " << params.jitter <<
		", originX = " << originX << ", originY = " << originY << std::endl;
	for (const auto& coordinate: resultCoordinates) {
		float x = originX + coordinate.x * params.gridSize + jitterDistribution(rng);
		float y = originY + coordinate.y * params.gridSize + jitterDistribution(rng);
		result.emplace_back(x, y);
	}

	return result;

}

}} /* namespace car::track */


