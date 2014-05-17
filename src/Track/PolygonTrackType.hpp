#ifndef POLYGONTRACKTYPE_HPP_
#define POLYGONTRACKTYPE_HPP_

#include "BasicTrackType.hpp"
#include "PolygonBasedTrackType.hpp"

namespace car {

class PolygonTrackType: public PolygonBasedTrackType {
public:
	PolygonTrackType();
	virtual std::function<Track()> getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	std::vector<std::string> points;
};

} /* namespace car */

#endif /* POLYGONTRACKTYPE_HPP_ */
