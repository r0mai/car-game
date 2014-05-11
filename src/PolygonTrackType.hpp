#ifndef POLYGONTRACKTYPE_HPP_
#define POLYGONTRACKTYPE_HPP_

#include <boost/program_options.hpp>
#include "ITrackType.hpp"

namespace car {

class PolygonTrackType: public ITrackType {
public:
	virtual std::function<Track()> getTrackCreator(const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::string getArgumentName() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
};

} /* namespace car */

#endif /* POLYGONTRACKTYPE_HPP_ */
