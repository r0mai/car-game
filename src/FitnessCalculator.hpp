#ifndef SRC_FITNESSCALCULATOR_HPP
#define SRC_FITNESSCALCULATOR_HPP

#include <boost/range/iterator_range.hpp>
#include "lua/Lua.hpp"

namespace car {

class FitnessCalculator {
public:
	FitnessCalculator(lua::Lua& lua): lua(lua) {}

	template <typename Iterator>
	float calculateFitness(Iterator begin, Iterator end) {
		lua::Table args;
		int i = 1;
		for (const auto& model: boost::make_iterator_range(begin, end)) {
			lua::Table arg;
			arg.emplace(std::string{"travelDistance"}, model.getCar().getTravelDistance());
			arg.emplace(std::string{"checkpoints"},
					static_cast<double>(model.getTrack().getNumberOfCheckpoints()));
			arg.emplace(std::string{"crossedCheckpoints"},
					static_cast<double>(model.getNumberOfCrossedCheckpoints()));

			args.emplace(static_cast<double>(i++), arg);
		}
		std::vector<lua::Data> results{{}};
		lua.callFunction("getFitness", {args}, &results);
		return boost::get<double>(results[0]);
	}
private:
	lua::Lua& lua;
};

}


#endif /* SRC_FITNESSCALCULATOR_HPP */
