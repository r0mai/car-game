#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class AIGameManager : public GameManager {
public:
	AIGameManager(const Parameters& parameters, track::TrackCreator trackCreator);

	void run();

	//should be called after run()
	float getFitness() const;

private:
	bool stopCondition() const;

	const float maxTime = 600.f;
};

}

#endif
