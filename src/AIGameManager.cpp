
#include "AIGameManager.hpp"

#include <ctime>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "randomUtil.hpp"

namespace car {

AIGameManager::AIGameManager(std::function<Track()> trackCreator) :
	GameManager(trackCreator) {}


void AIGameManager::run() {
	while (true) {
		advance();
		if (model.hasCarCollided()) {
			break;
		}
	}
}

}

