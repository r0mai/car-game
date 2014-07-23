#include "ScreenDimension.hpp"
#include <boost/spirit/include/qi.hpp>
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace car {

ScreenDimension parseScreenDimenstion(const std::string& s) {
	ScreenDimension result;
	auto meterSetter = [&](float f) { result = Meters{f}; };
	auto percentSetter = [&](float f) { result = Percent{f}; };
	auto pixelSetter = [&](unsigned i) { result = Pixels{i}; };

	auto it = s.begin();
	bool success = qi::phrase_parse(it, s.end(),
		(
		   	( qi::float_ >> "m" )[meterSetter] |
			( qi::float_ >> "%" )[percentSetter] |
			( qi::uint_ >> "p" >> -qi::lit("x") )[pixelSetter]
		), boost::spirit::ascii::space);

	if (!success || it != s.end()) {
		std::ostringstream ss;
		ss << "Lexical error parsing screen dimension:\n" << s << "\n";
		for (auto it2 = s.begin(); it2 != it; ++it2) {
			ss << ' ';
		}
		ss << '^';
		throw std::logic_error{ss.str()};
	}

	return result;
}


}

