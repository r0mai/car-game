#ifndef SIMPLEPOLYGONTYPE_HPP_
#define SIMPLEPOLYGONTYPE_HPP_

#include "BasicPolygonType.hpp"

namespace car {

class SimplePolygonType: public BasicPolygonType {
public:
	SimplePolygonType();
	virtual std::function<Track(const std::vector<sf::Vector2f>)> getTrackCreator(
			const boost::program_options::variables_map& variablesMap) override;
	virtual std::string getHelpString() override;
private:
	struct Params {
		float checkpointDistance = 5.f;
		float trackWidth = 5.f;
	};
	Params params;
};

}



#endif /* SIMPLEPOLYGONTYPE_HPP_ */
