
#include "GameManager.hpp"

#include <iostream>

namespace car {

using namespace irr;

GameManager::GameManager() :
	device(createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(640, 480), 16, false, false, false, 0)),
	car()
{
}

GameManager::~GameManager() {
	device->drop();
}

void GameManager::run() {
	video::IVideoDriver *driver = device->getVideoDriver();
	scene::ISceneManager *smgr = device->getSceneManager();
	gui::IGUIEnvironment *guienv = device->getGUIEnvironment();

	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setPosition(core::vector3df(0, 0, 100));
	camera->setTarget(core::vector3df(0, 0, 0));

	scene::IMeshSceneNode *carMesh = smgr->addCubeSceneNode();
	car.setMesh(carMesh);

	u32 then = device->getTimer()->getTime();

	while(device->run()) {
		const u32 now = device->getTimer()->getTime();
		const f32 deltaSeconds = (now - then)/1000.f;
		then = now;

		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
}

}

