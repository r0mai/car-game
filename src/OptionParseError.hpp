#ifndef OPTIONPARSEERROR_HPP_
#define OPTIONPARSEERROR_HPP_

#include <stdexcept>

namespace car {

struct OptionParseError: std::logic_error {
	using std::logic_error::logic_error;
};


}



#endif /* OPTIONPARSEERROR_HPP_ */
