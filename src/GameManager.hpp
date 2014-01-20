#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <irrlicht/irrlicht.h>

#include <vector>
#include "Car.hpp"

namespace car {

class GameManager {
public:
	GameManager();
	~GameManager();

	void run();

private:
	void drawTelemetry(irr::f32 deltaSeconds);

	irr::IrrlichtDevice *device = nullptr;


	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment *guienv = device->getGUIEnvironment();

	irr::gui::IGUIFont *font = guienv->getBuiltInFont();

	std::vector<irr::core::vector2df> speeds;

	Car car;
};

}

#endif /* !GAMEMANAGER_HPP */
