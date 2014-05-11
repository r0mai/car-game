#ifndef OPTIONSHELPER_HPP_
#define OPTIONSHELPER_HPP_

#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/options_description.hpp>
#include <SFML/System/Vector2.hpp>

namespace car {

template <typename Params, typename T>
boost::program_options::typed_value<T>* optionalParamValue(T Params::* param, Params* params) {
	namespace po = boost::program_options;
	if (params) {
		return po::value<T>(&(params->*param));
	} else {
		return po::value<T>();
	}
}

template <typename Params, typename T>
boost::program_options::typed_value<T>* optionalParamValueWithDefaultValue(T Params::* param, Params* params) {
	auto result = optionalParamValue(param, params);
	if (params) {
		return result->default_value(params->*param);
	} else {
		return result;
	}
}

sf::Vector2f parsePoint(const std::string& param);

void parseConfigFile(const std::string& filename,
		const boost::program_options::options_description& optionsDescription);

}

#endif /* OPTIONSHELPER_HPP_ */
