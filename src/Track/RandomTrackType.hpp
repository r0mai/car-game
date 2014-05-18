#ifndef RANDOMTRACKTYPE_HPP_
#define RANDOMTRACKTYPE_HPP_

#include "PolygonBasedTrackType.hpp"
#include "RandomTrackGenerator.hpp"

namespace car { namespace track {

class RandomTrackType: public PolygonBasedTrackType {
public:
	RandomTrackType();
	virtual std::function<Track()> getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	std::string corner1 = "-60.0,-60.0";
	std::string corner2 = "60.0,60.0";
	RandomTrackGenerator::Params generatorParams;
};

}} /* namespace car::track */


#endif /* RANDOMTRACKTYPE_HPP_ */
