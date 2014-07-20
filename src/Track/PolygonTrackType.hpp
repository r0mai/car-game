#ifndef POLYGONTRACKTYPE_HPP_
#define POLYGONTRACKTYPE_HPP_

#include "BasicTrackType.hpp"
#include "PolygonBasedTrackType.hpp"

namespace car { namespace track {

class PolygonTrackType: public PolygonBasedTrackType {
public:
	PolygonTrackType();
	virtual TrackCreator getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	std::vector<std::string> points;
};

}} /* namespace car::track */

#endif /* POLYGONTRACKTYPE_HPP_ */
