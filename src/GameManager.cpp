
#include "GameManager.hpp"

#include <iostream>
#include <sstream>

namespace car {

using namespace irr;

GameManager::GameManager() :
	device(createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(1024, 768), 16, false, false, false, 0)),
	car()
{
}

GameManager::~GameManager() {
	device->drop();
}

void GameManager::run() {

	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setPosition(core::vector3df(0, 500, 0));
	camera->setTarget(core::vector3df(0, 0, 0));

	scene::IMeshSceneNode *carMesh = smgr->addCubeSceneNode();
	car.setMesh(carMesh);

	u32 then = device->getTimer()->getTime();

	while(device->run()) {
		const u32 now = device->getTimer()->getTime();
		const f32 deltaSeconds = (now - then)/1000.f;
		then = now;

		car.move(deltaSeconds);

		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();
		drawTelemetry(deltaSeconds);

		driver->endScene();
	}
}

void GameManager::drawTelemetry(f32 deltaSeconds) {
	std::wstringstream ss;
	ss << L"Speed = " << car.getSpeed();
	font->draw(ss.str().c_str(), core::rect<s32>(10, 10, 100, 100), video::SColor(0, 0, 0, 255));
	speeds.push_back(core::vector2df(speeds.empty() ? deltaSeconds : speeds.back().X + deltaSeconds, car.getSpeed()));
	for ( auto p : speeds ) {
		s32 x = p.X*10;
		s32 y = p.Y*100 - 1000;
		driver->drawPixel(x, y, video::SColor(255, 0, 0, 255));
	}

}

}

