#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <irrlicht/irrlicht.h>

#include "Car.hpp"

namespace car {

class GameManager {
public:
	GameManager();
	~GameManager();

	void run();

private:
	irr::IrrlichtDevice *device = nullptr;

	Car car;
};

}

#endif /* !GAMEMANAGER_HPP */
