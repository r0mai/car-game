#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"
#include "Track/TrackCreator.hpp"
#include "MathExpression.hpp"

namespace car {

class AIGameManager : public GameManager {
public:
	AIGameManager(const CommonParameters& parameters, track::TrackCreator trackCreator,
			const MathExpression& fitnessExpression);

	void run();

	//should be called after run()
	float getFitness() const;

private:
	bool stopCondition() const;

	const float maxTime = 600.f;
	MathExpression fitnessExpression;
};

}

#endif
