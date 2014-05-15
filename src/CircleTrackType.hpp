#ifndef CIRCLETRACKTYPE_HPP_
#define CIRCLETRACKTYPE_HPP_

#include "ITrackType.hpp"
#include "createCircleTrack.hpp"

namespace car {

class CircleTrackType: public ITrackType {
public:
	CircleTrackType();
	virtual std::function<Track()> getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::string getArgumentName() override;
	virtual boost::program_options::options_description getOptions() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	CircleTrackParams params;
	boost::program_options::options_description optionsDescription;
};

} /* namespace car */




#endif /* CIRCLETRACKTYPE_HPP_ */
