
#include "Neuron.hpp"

#include "randomUtil.hpp"
#include "mathUtil.hpp"

namespace car {

Neuron::Neuron(unsigned inputCount, bool hasRecurrence)	:
	weights(inputCount + 1 + hasRecurrence), recurrence(hasRecurrence, 0)
{
	for (Weight& weight : weights) {
		weight = randomReal(-1, 1);
	}
}

Weight Neuron::run(const Weights& input) {
	assert(weights.size() - 1 - static_cast<bool>(recurrence) == input.size());

	Weight netInput = 0;

	unsigned i = 0;
	while (i < input.size()) {
		netInput += weights[i]*input[i];
		++i;
	}
	if (recurrence) {
		netInput += (*recurrence)*weights[i++];
	}
	netInput += -1.f*weights[i++];

	Weight sigmoid = sigmoidApproximation(netInput);
	if (recurrence) {
		recurrence = sigmoid;
	}
	return sigmoid;
}

}

