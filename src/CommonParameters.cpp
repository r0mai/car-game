#include "CommonParameters.hpp"
#include "NeuralNetwork.hpp"

#include <boost/lexical_cast.hpp>

namespace car {

void setNeuralNetworkExternalParameters(const CommonParameters& parameters, NeuralNetwork& neuralNetwork) {
	neuralNetwork.setExternalParameter("lookaheadType", boost::lexical_cast<std::string>(parameters.lookaheadType));
	neuralNetwork.setExternalParameter("rayCount", boost::lexical_cast<std::string>(parameters.rayCount));
	neuralNetwork.setExternalParameter("checkpointLookAhead", boost::lexical_cast<std::string>(parameters.checkpointLookAhead));
}

namespace {

template <typename T>
T getParameter(const std::string& s) {
	return s.empty() ? T{} : boost::lexical_cast<T>(s);
}

}

void getNeuralNetworkExternalParameters(CommonParameters& parameters, const NeuralNetwork& neuralNetwork) {
	parameters.lookaheadType = getParameter<LookaheadType>(neuralNetwork.getExternalParameter("lookaheadType"));
	parameters.rayCount = getParameter<int>(neuralNetwork.getExternalParameter("rayCount"));
	parameters.checkpointLookAhead = getParameter<int>(neuralNetwork.getExternalParameter("checkpointLookAhead"));
}

}


