#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"

namespace car {

class AIGameManager : public GameManager {
public:
	AIGameManager(std::function<Track()> trackCreator);

	void run();

	//should be called after run()
	float getFitness() const;

private:
	bool stopCondition() const;

	unsigned currentIteration = 0;
	const float maxTime = 600.f;
};

}

#endif
