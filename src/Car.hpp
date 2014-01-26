#ifndef CAR_HPP
#define CAR_HPP

#include <SFML/Graphics.hpp>

namespace car {

class Car {
public:


	Car() = default;
	Car(const sf::Vector2f& position);

	void move(float deltaSeconds);

	void setThrottle(float value);
	float getThrottle() const;
	void increaseThrottle(float deltaSeconds);
	void decreaseThrottle(float deltaSeconds);

	void setBrake(float value);
	float getBrake() const;
	void increaseBrake(float deltaSeconds);
	void decreaseBrake(float deltaSeconds);

	void increaseTurnToRight(float deltaSeconds);
	void increaseTurnToLeft(float deltaSeconds);
	void dontTurn(float deltaSeconds);

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getVelocity() const;
	float getSpeed() const;

	const sf::Vector2f& getAcceleration() const;

private:
	float throttleLevel = 0.0; // [0, 1]
	float brakeLevel = 0.0; // [0, 1]
	float turnLevel = 0.0; // [-1, 1] => [left, right]

	sf::Vector2f position = sf::Vector2f(0, 0); //unit is m

	sf::Vector2f velocity = sf::Vector2f(0, 0); //in m/s
	sf::Vector2f orientation = sf::Vector2f(0, -1); //unit vector
	sf::Vector2f acceleration; //recalculated with move();
};

}

#endif /* !CAR_HPP */
