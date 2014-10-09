#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <memory>

#include <boost/optional.hpp>

#include "Car.hpp"
#include "Track/Track.hpp"

namespace car {

class Model {
public:
	Model(const CarParameters& carParameters):
		car{carParameters} {}

	void setCar(const Car& newCar);
	void setTrack(std::shared_ptr<const track::Track> newTrack);

	const Car& getCar() const;
	Car& getCar();
	const std::shared_ptr<const track::Track>& getTrack() const;

	void setRightPressed(bool isPressed);
	void setLeftPressed(bool isPressed);
	void setForwardPressed(bool isPressed);
	void setBackwardPressed(bool isPressed);

	float getCurrentTime() const;

	bool hasCarCollided() const;

	int getCurrentCheckpoint() const { return currentCheckpoint; }

	std::vector<boost::optional<sf::Vector2f>> getRayPoints(unsigned count) const;

	unsigned getNumberOfCrossedCheckpoints() const;

	void advanceTime(float deltaSeconds);

	struct CheckpointInformation {
		sf::Vector2f orientation;
		float leftEdgeDistance;
		sf::Vector2f leftEdgeOrientation;
		float rightEdgeDistance;
		sf::Vector2f rightEdgeOrientation;
	};

	auto getCheckpointInformation(unsigned count) const -> std::vector<CheckpointInformation>;

private:
	void collideCar();
	void handleCheckpoints();
	bool findFirstCheckpoint();
	void handleInput(float deltaSeconds);
	bool collidesWithCheckpoint(std::size_t checkpointId);

	Car car;
	std::shared_ptr<const track::Track> track;

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
