#ifndef ITRACKTYPE_HPP_
#define ITRACKTYPE_HPP_

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include "TrackCreator.hpp"

namespace car { namespace track {

class Track;

class ITrackType {
public:
	virtual TrackCreator getTrackCreator(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) = 0;
	virtual bool needsReparse(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) = 0;
	virtual std::string getHelpString() = 0;
	virtual std::string getArgumentName() = 0;
	virtual boost::program_options::options_description getOptions() = 0;
	virtual std::size_t getMinimumNumberOfArgs() = 0;
	virtual ~ITrackType() {}
};

}} /* namespace car::track */



#endif /* ITRACKTYPE_HPP_ */
