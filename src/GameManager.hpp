#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <irrlicht/irrlicht.h>
#include <vector>

#include "Car.hpp"
#include "Telemetry.hpp"

namespace car {

class GameManager : public irr::IEventReceiver {
public:
	GameManager();
	~GameManager();

	virtual bool OnEvent(const irr::SEvent& event) override;

	void run();

private:
	void handleInput(irr::f32 deltaSeconds);
	void drawTelemetry();
	void updateTelemetry();

	irr::IrrlichtDevice *device = nullptr;

	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment *guienv = device->getGUIEnvironment();

	irr::gui::IGUIFont *font = guienv->getBuiltInFont();

	irr::f32 currentTime = 0.f;

	Car car;
	Telemetry speedTelemetry;
	Telemetry accelerationTelemetry;
	Telemetry gasTelemetry;
	Telemetry brakeTelemetry;

	bool pressedKeys[irr::KEY_KEY_CODES_COUNT] = {false};
};

}

#endif /* !GAMEMANAGER_HPP */
