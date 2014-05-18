#ifndef BASICARGUMENTPARSEDTYPE_HPP_
#define BASICARGUMENTPARSEDTYPE_HPP_

#include <string>
#include <boost/program_options/options_description.hpp>

namespace car { namespace track {

template <typename Interface>
class BasicArgumentParsedType: public Interface {
public:
	BasicArgumentParsedType(std::string argumentName):
			argumentName(std::move(argumentName))
	{}
	virtual std::string getArgumentName() override {
		return argumentName;
	}
	virtual boost::program_options::options_description getOptions() override {
		return optionsDescription;
	}
protected:
	boost::program_options::options_description optionsDescription;
private:
	std::string argumentName;
};

}} /* namespace car::track */



#endif /* BASICARGUMENTPARSEDTYPE_HPP_ */
