#ifndef CAR_HPP
#define CAR_HPP

#include <SFML/Graphics.hpp>

namespace car {

class Car {
public:


	Car();
	Car(const sf::Vector2f& position);

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

	void draw(sf::RenderWindow& window) const;

private:

	void updateCorners();

	static const float cDrag;
	static const float cRollingResistance;
	static const float fEngine;
	static const float fBrake;
	static const float gravity;
	static const float transMissionEfficiency;
	static const float gearRatio;
	static const float differentialRatio;
	static const float wheelRadius;
	static const float mass;
	static const float maxTurnAngle;
	static const float turnRate;

	//CM == Center of Mass
	//CG == Center of Gravity
	static const float rearWheelCMDistance;
	static const float frontWheelCMDistance;
	static const float rearCMDistance;
	static const float frontCMDistance;
	static const float heightOfCG;
	static const float wheelBase;
	static const float carWidth;

	static const float throttleIncreaseSpeed;
	static const float throttleDecreaseSpeed;
	static const float brakeIncreaseSpeed;
	static const float brakeDecreaseSpeed;

	static const float turnSpeed;

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

	sf::Color color = sf::Color::White;
};

}

#endif /* !CAR_HPP */
