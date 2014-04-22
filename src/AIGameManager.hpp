#ifndef AIGAMEMANAGER_HPP
#define AIGAMEMANAGER_HPP

#include "GameManager.hpp"

namespace car {

class AIGameManager : public GameManager {
public:
	AIGameManager(std::function<Track()> trackCreator);

	void run();
};

}

#endif
