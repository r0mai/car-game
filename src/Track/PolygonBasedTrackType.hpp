#ifndef POLYGONBASEDTRACKTYPE_HPP_
#define POLYGONBASEDTRACKTYPE_HPP_

#include "BasicTrackType.hpp"
#include "IPolygonType.hpp"

namespace car {

class PolygonBasedTrackType: public BasicTrackType {
public:
	PolygonBasedTrackType(const std::string& argumentName);
	virtual bool needsReparse(
			const boost::program_options::variables_map& variablesMap,
			const std::vector<std::string>& args) override;
	virtual boost::program_options::options_description getOptions() override;

	const std::shared_ptr<IPolygonType>& getPolygonType() { return polygonType; }
private:
	std::shared_ptr<IPolygonType> polygonType;
	std::string polygonTypeName;
};

} /* namespace car */




#endif /* POLYGONBASEDTRACKTYPE_HPP_ */
