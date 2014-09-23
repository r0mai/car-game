#ifndef CAR_HPP
#define CAR_HPP

#include <SFML/Graphics.hpp>
#include "CarParameters.hpp"

namespace car {

class Car {
public:


	Car(const CarParameters& parameters);
	Car(const CarParameters& parameters, const sf::Vector2f& position, float direction);

	void setColor(const sf::Color& color);

	void move(float deltaSeconds);

	void setThrottle(float value);
	float getThrottle() const;
	void increaseThrottle(float deltaSeconds);
	void decreaseThrottle(float deltaSeconds);

	void setBrake(float value);
	float getBrake() const;
	void increaseBrake(float deltaSeconds);
	void decreaseBrake(float deltaSeconds);

	void setTurnLevel(float value);
	float getTurnLevel() const;
	void increaseTurnToRight(float deltaSeconds);
	void increaseTurnToLeft(float deltaSeconds);
	void dontTurn(float deltaSeconds);

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getVelocity() const;
	const sf::Vector2f& getOrientation() const;
	float getSpeed() const;

	const sf::Vector2f& getAcceleration() const;

	const sf::Vector2f& getFrontLeftCorner() const;
	const sf::Vector2f& getFrontRightCorner() const;
	const sf::Vector2f& getRearLeftCorner() const;
	const sf::Vector2f& getRearRightCorner() const;

	float getTravelDistance() const;

	void draw(sf::RenderWindow& window) const;

private:

	void updateCorners();

	CarParameters parameters;

	float throttleLevel = 0.0; // [0, 1]
	float brakeLevel = 0.0; // [0, 1]
	float turnLevel = 0.0; // [-1, 1] => [left, right]

	sf::Vector2f position = sf::Vector2f(0, 0); //unit is m

	sf::Vector2f velocity = sf::Vector2f(0, 0); //in m/s
	sf::Vector2f orientation = sf::Vector2f(1., 0.); //unit vector
	sf::Vector2f acceleration; //recalculated with move();

	sf::Vector2f frontLeftCorner; //recalculated with move()
	sf::Vector2f frontRightCorner; //recalculated with move()
	sf::Vector2f rearLeftCorner; //recalculated with move()
	sf::Vector2f rearRightCorner; //recalculated with move()

	float travelDistance = 0.f; //recalculated with move()

	sf::Color color = sf::Color::White;
};

}

#endif /* !CAR_HPP */
