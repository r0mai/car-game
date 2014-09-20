#ifndef NEURONWEIGHTS_HPP
#define NEURONWEIGHTS_HPP

#include <vector>
#include <memory>

namespace car {

typedef float Weight;
typedef std::vector<Weight> Weights;

// do not use serialization here. It makes little sense to save a shared
// pointer and it would break save compatibility
template<class Archive>
std::shared_ptr<Weights> loadWeights(Archive & ar)
{
	auto result = std::make_shared<car::Weights>();
	ar >> *result;
	return std::move(result);
}

}

#endif /* !NEURONWEIGHTS_HPP */
