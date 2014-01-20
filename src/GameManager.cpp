
#include "GameManager.hpp"

#include <iostream>
#include <sstream>

namespace car {

using namespace irr;

GameManager::GameManager() :
	device(createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(1024, 768), 16, false, false, false, this)),
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
		currentTime += deltaSeconds;
		then = now;
		if (pressedKeys[KEY_UP]) {
			car.increaseGas(deltaSeconds);
		} else {
			car.decreaseGas(deltaSeconds);
		}

		car.move(deltaSeconds);
		updateTelemetry();

		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();
		drawTelemetry();

		driver->endScene();
	}
}

bool GameManager::OnEvent(const irr::SEvent& event) {
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		pressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
	return false;
}

void GameManager::updateTelemetry() {
	speedTelemetry.addDataPoint(core::vector2df(currentTime, car.getSpeed()));
	gasTelemetry.addDataPoint(core::vector2df(currentTime, car.getGas()));

}

void GameManager::drawTelemetry() {
	std::wstringstream ss; ss << L"Speed = " << car.getSpeed() << L", Gas = " << car.getGas();
	font->draw(ss.str().c_str(), core::rect<s32>(10, 10, 100, 100), video::SColor(0, 0, 0, 255));

	speedTelemetry.drawAsGraph(driver, core::recti(10, 20, 1014, 220));
	gasTelemetry.drawAsGraph(driver, core::recti(10, 230, 1014, 430));
}

}

