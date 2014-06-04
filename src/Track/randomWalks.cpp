#include "randomWalks.hpp"
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/map.hpp>
#include "WilsonRandomWalk.hpp"
#include "PrimRandomWalk.hpp"
#include "DfsRandomWalk.hpp"
#include "TrackCreatorError.hpp"
#include "LazyArgumentMap.hpp"

namespace car { namespace track {

namespace randomWalk {

namespace {

LAZY_ARGUMENT_MAP(BaseRandomWalk, randomWalkTypes) {
	return {
		{"wilson", std::make_shared<WilsonRandomWalk>()},
		{"prim", std::make_shared<PrimRandomWalk>()},
		{"dfs", std::make_shared<DfsRandomWalk>()},
	};
}

};

std::shared_ptr<BaseRandomWalk> getRandomWalk(const std::string& name) {
	auto it = randomWalkTypes().find(name);
	if (it == randomWalkTypes().end()) {
		throw TrackCreatorError{"Invalid random walk algorithm: " + name};
	}

	return it->second;
}

std::string getHelpString() {
	return "Allowed values: " + boost::algorithm::join(randomWalkTypes() |
			boost::adaptors::map_keys, ", ") + ".";
}


} // namespace randomWalk

}} /* namespace car::track */


