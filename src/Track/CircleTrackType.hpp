#ifndef CIRCLETRACKTYPE_HPP_
#define CIRCLETRACKTYPE_HPP_

#include "BasicTrackType.hpp"
#include "createCircleTrack.hpp"

namespace car { namespace track {

class CircleTrackType: public BasicTrackType {
public:
	CircleTrackType();
	virtual std::function<Track()> getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual bool needsReparse(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	CircleTrackParams params;
};

}} /* namespace car::track */




#endif /* CIRCLETRACKTYPE_HPP_ */
