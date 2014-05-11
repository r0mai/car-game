#ifndef CIRCLETRACKTYPE_HPP_
#define CIRCLETRACKTYPE_HPP_

#include "ITrackType.hpp"

namespace car {

class CircleTrackType: public ITrackType {
public:
	virtual std::function<Track()> getTrackCreator(const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::string getArgumentName() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
};

} /* namespace car */




#endif /* CIRCLETRACKTYPE_HPP_ */
