#ifndef SRC_CARINPUTPARAMETERS_HPP
#define SRC_CARINPUTPARAMETERS_HPP

#include <string>
#include <boost/optional.hpp>

namespace car {

struct CarInputParameters {
	boost::optional<std::string> neuralNetworkFile;
};

}


#endif /* SRC_CARINPUTPARAMETERS_HPP */
