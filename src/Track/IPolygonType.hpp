#ifndef IPOLYGONTYPE_HPP_
#define IPOLYGONTYPE_HPP_

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <SFML/Graphics.hpp>

namespace car {

class Track;

class IPolygonType {
public:
	virtual std::function<Track(const std::vector<sf::Vector2f>)> getTrackCreator(
			const boost::program_options::variables_map& variablesMap) = 0;
	virtual std::string getHelpString() = 0;
	virtual std::string getArgumentName() = 0;
	virtual boost::program_options::options_description getOptions() = 0;
	virtual ~IPolygonType() {}
};

template <typename PolygonType>
std::pair<std::string, std::shared_ptr<IPolygonType>>
createPolygonTypeElement() {
	auto polygonType = std::make_shared<PolygonType>();
	auto argumentName = polygonType->getArgumentName();
	return {std::move(argumentName), std::move(polygonType)};
}

}



#endif /* IPOLYGONTYPE_HPP_ */
