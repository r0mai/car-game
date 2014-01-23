
#include "GameManager.hpp"

#include <iostream>
#include <sstream>

namespace car {

using namespace irr;

GameManager::GameManager() :
	device(createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(1024, 768), 16, false, false, false, this)),
	car()
{
	gasTelemetry.setAutomaticBoundsDetection(false);
	gasTelemetry.setBounds(0.f, 1.f);
	brakeTelemetry.setAutomaticBoundsDetection(false);
	brakeTelemetry.setBounds(0.f, 1.f);
}

GameManager::~GameManager() {
	device->drop();
}

void GameManager::run() {

	scene::IAnimatedMesh *carMesh = smgr->getMesh("models/car.lwo");
	scene::IAnimatedMeshSceneNode *carMeshNode = smgr->addAnimatedMeshSceneNode(carMesh);

	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->bindTargetAndRotation(true);
	camera->setPosition(core::vector3df(0, 5, 15));
	camera->setTarget(carMeshNode->getPosition());

	car.setMesh(carMeshNode);

	u32 then = device->getTimer()->getTime();

	while(device->run()) {
		const u32 now = device->getTimer()->getTime();
		const f32 deltaSeconds = (now - then)/1000.f;
		currentTime += deltaSeconds;
		then = now;

		handleInput(deltaSeconds);
		car.move(deltaSeconds);
		updateTelemetry();

		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();
		drawTelemetry();

		driver->endScene();
	}
}

void GameManager::handleInput(irr::f32 deltaSeconds) {
	if (pressedKeys[KEY_UP]) {
		car.increaseGas(deltaSeconds);
	} else {
		car.decreaseGas(deltaSeconds);
	}
	if (pressedKeys[KEY_DOWN]) {
		car.increaseBrake(deltaSeconds);
	} else {
		car.decreaseBrake(deltaSeconds);
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
	accelerationTelemetry.addDataPoint(core::vector2df(currentTime, car.getAcceleration().X));
	gasTelemetry.addDataPoint(core::vector2df(currentTime, car.getGas()));
	brakeTelemetry.addDataPoint(core::vector2df(currentTime, car.getBrake()));
}

void GameManager::drawTelemetry() {
	std::wstringstream ss; ss << L"Speed = " << car.getSpeed() << L", Acceleration = " << car.getAcceleration().X << L", Gas = " << car.getGas() << L", Brake = " << car.getBrake();
	font->draw(ss.str().c_str(), core::rect<s32>(10, 10, 100, 100), video::SColor(0, 0, 0, 255));

	speedTelemetry.drawAsGraph(driver, core::recti(10, 20, 1014, 220));
	accelerationTelemetry.drawAsGraph(driver, core::recti(10, 20, 1014, 220));
	gasTelemetry.drawAsGraph(driver, core::recti(10, 230, 1014, 430));
	brakeTelemetry.drawAsGraph(driver, core::recti(10, 230, 1014, 430));
}

}

