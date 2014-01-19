
#include "GameManager.hpp"

#include <iostream>

namespace car {

GameManager::GameManager() :
	device(irr::createDevice(irr::video::EDT_SOFTWARE, irr::core::dimension2d<irr::u32>(640, 480), 16, false, false, false, 0))
{
}

GameManager::~GameManager() {
	device->drop();
}

void GameManager::run() {
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", irr::core::rect<irr::s32>(10,10,260,22), true);
	while(device->run()) {
		driver->beginScene(true, true, irr::video::SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
}

}

