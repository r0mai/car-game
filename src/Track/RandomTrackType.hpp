#ifndef RANDOMTRACKTYPE_HPP_
#define RANDOMTRACKTYPE_HPP_

#include "PolygonBasedTrackType.hpp"
#include "RandomTrackGenerator.hpp"
#include "IRandomPolygonGeneratorType.hpp"

namespace car { namespace track {

class RandomTrackType: public PolygonBasedTrackType {
public:
	RandomTrackType();
	virtual TrackCreator getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual std::string getHelpString() override;
	virtual bool needsReparse(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual boost::program_options::options_description getOptions() override;
	virtual std::size_t getMinimumNumberOfArgs() override;
private:
	std::string algorithm;
	std::shared_ptr<IRandomPolygonGeneratorType> polygonGeneratorType;
	RandomTrackGenerator::Params generatorParams;
};

}} /* namespace car::track */


#endif /* RANDOMTRACKTYPE_HPP_ */
