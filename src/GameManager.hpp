#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <irrlicht/irrlicht.h>

namespace car {

class GameManager {
public:
	GameManager();
	~GameManager();

	void run();

private:
	irr::IrrlichtDevice *device = nullptr;
};

}

#endif /* !GAMEMANAGER_HPP */
