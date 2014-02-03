
#include "randomUtil.hpp"

#include <cstdlib>
#include <cassert>

namespace car {

float randomReal(float min, float max) {
	assert(max >= min);
	return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX/(max - min));
}

int randomInt(int min, int max) {
	return std::rand()%(max - min + 1) + min;
}

}

