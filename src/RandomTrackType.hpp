#ifndef RANDOMTRACKTYPE_HPP_
#define RANDOMTRACKTYPE_HPP_

#include "BasicTrackType.hpp"
#include "RandomTrackGenerator.hpp"

namespace car {

class RandomTrackType: public BasicTrackType {
public:
	RandomTrackType();
	virtual std::function<Track()> getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	struct Params {
		float checkpointDistance = 5.f;
		float trackWidth = 5.f;
		std::string corner1 = "-60.0,-60.0";
		std::string corner2 = "60.0,60.0";
		RandomTrackGenerator::Params generatorParams;
	};
	Params params;
};

} /* namespace car */


#endif /* RANDOMTRACKTYPE_HPP_ */
