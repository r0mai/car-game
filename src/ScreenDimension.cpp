#include "ScreenDimension.hpp"
#include <boost/spirit/include/qi.hpp>
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

namespace car {

namespace {

template <typename Iterator>
	class ScreenDimensionParser: public qi::grammar<Iterator, ScreenDimension(), ascii::space_type> {
	public:
		ScreenDimensionParser(): ScreenDimensionParser::base_type{start} {
			meters %= qi::float_ >> "m";
			percent %= qi::float_ >> "%";
			pixels %= qi::uint_ >> "p" >> -qi::lit("x");
			start %= meters | percent | pixels;
		}
	private:
		qi::rule<Iterator, ScreenDimension(), ascii::space_type> start;
		qi::rule<Iterator, Meters(), ascii::space_type> meters;
		qi::rule<Iterator, Percent(), ascii::space_type> percent;
		qi::rule<Iterator, Pixels(), ascii::space_type> pixels;
	};

}


ScreenDimension parseScreenDimenstion(const std::string& s) {
	ScreenDimension result;
	auto it = s.begin();
	ScreenDimensionParser<decltype(it)> parser;
	bool success = qi::phrase_parse(it, s.end(), parser, ascii::space, result);

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

