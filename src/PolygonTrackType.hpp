#ifndef POLYGONTRACKTYPE_HPP_
#define POLYGONTRACKTYPE_HPP_

#include "BasicTrackType.hpp"

namespace car {

class PolygonTrackType: public BasicTrackType {
public:
	PolygonTrackType();
	virtual std::function<Track()> getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	struct Params {
		float checkpointDistance = 5.f;
		float trackWidth = 5.f;
		std::vector<std::string> points;
	};
	Params params;
};

} /* namespace car */

#endif /* POLYGONTRACKTYPE_HPP_ */
