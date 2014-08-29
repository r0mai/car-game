#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include <boost/optional.hpp>

#include "Car.hpp"
#include "Track/Track.hpp"

namespace car {

class Model {
public:
	Model();

	void setCar(const Car& newCar);
	void setTrack(const track::Track& newTrack);

	const Car& getCar() const;
	Car& getCar();
	const track::Track& getTrack() const;
	track::Track& getTrack();

	void setRightPressed(bool isPressed);
	void setLeftPressed(bool isPressed);
	void setForwardPressed(bool isPressed);
	void setBackwardPressed(bool isPressed);

	float getCurrentTime() const;

	bool hasCarCollided() const;

	std::vector<boost::optional<sf::Vector2f>> getRayPoints(unsigned count) const;

	unsigned getNumberOfCrossedCheckpoints() const;

	void advanceTime(float deltaSeconds);

	void drawTrack(sf::RenderWindow& window, bool drawCheckpoints = true) const;

	sf::Vector2f getCheckpointDirection() const;

	float getOutTime() { return outTime; }

private:
	void collideCar();
	void handleCheckpoints();
	bool findFirstCheckpoint();
	void handleInput(float deltaSeconds);
	bool collidesWithCheckpoint(std::size_t checkpointId);

	Car car;
	track::Track track;
	float outTime = 0.0;

	bool isCarCollided = false;
	float currentTime = 0.f;
	int currentCheckpoint = -1;
	unsigned numberOfCrossedCheckpoints = 0;

	bool rightPressed = false;
	bool leftPressed = false;
	bool forwardPressed = false;
	bool backwardPressed = false;
};

}

#endif
