#ifndef SRC_MATRIXADAPTOR_HPP
#define SRC_MATRIXADAPTOR_HPP

#include <cstdint>
#include <limits>
#include <SFML/Graphics.hpp>

namespace car {

using MatrixCoordinate = sf::Vector2<std::size_t>;

struct MatrixAdaptor {
public:
	MatrixAdaptor():width(0), height(0) {}
	MatrixAdaptor(std::size_t width, std::size_t height):
			width(width), height(height)
	{}

	std::size_t getWidth() const { return width; }
	std::size_t getHeight() const { return height; }

	std::size_t operator[](const MatrixCoordinate& p) {
		if (p.x < width && p.y < height) {
			return p.y * width + p.x;
		} else {
			return outsideRange();
		}
	}

	static constexpr std::size_t outsideRange() {
		return std::numeric_limits<std::size_t>::max();
	};
private:
	std::size_t width;
	std::size_t height;
};

} // namespace car


#endif /* SRC_MATRIXADAPTOR_HPP */
