#ifndef BASICTRACKTYPE_HPP_
#define BASICTRACKTYPE_HPP_

#include "ITrackType.hpp"

namespace car {

class BasicTrackType: public ITrackType {
public:
	BasicTrackType(std::string argumentName);
	virtual std::string getArgumentName() override;
	virtual boost::program_options::options_description getOptions() override;
protected:
	boost::program_options::options_description optionsDescription;
private:
	std::string argumentName;
};

} /* namespace car */



#endif /* BASICTRACKTYPE_HPP_ */
