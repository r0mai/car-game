#include "optionsUtil.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include "OptionParseError.hpp"

namespace po = boost::program_options;

namespace car {

sf::Vector2f parsePoint(const std::string& param) {
	namespace algo = boost::algorithm;

	std::vector<std::string> tokens;
	algo::split(tokens, param, [](char ch) { return ch == ','; });

	if (tokens.size() != 2) {
		throw OptionParseError{"Point must be of format \"x,y\""};
	}
	algo::trim(tokens[0]);
	algo::trim(tokens[1]);
	return {boost::lexical_cast<float>(tokens[0]), boost::lexical_cast<float>(tokens[1])};
}

}


