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

	const float cDrag = 0.5;
	const float cRollingResistance = 14.2;
	const float fEngine = 2000.0;
	const float fBrake = 3000.0;
	const float gravity = 9.8; //m/s^2
	const float transMissionEfficiency = 0.7;
	const float gearRatio = 2.1;
	const float differentialRatio = 3.42;
	const float wheelRadius = 0.34; //m
	const float mass = 1500; //kg
	const float maxTurnAngle = 0.52; //radians ~= 30 degrees

	//CM == Center of Mass
	//CG == Center of Gravity
	const float rearWheelCMDistance = 1.0;
	const float frontWheelCMDistance = 1.5;
	const float rearCMDistance = 1.2;
	const float frontCMDistance = 1.8;
	const float heightOfCG = 1.5;
	const float wheelBase = rearCMDistance + frontCMDistance;
	const float carWidth = 1.4;

	const float throttleIncreaseSpeed = 1.5;
	const float throttleDecreaseSpeed = 1.3;
	const float brakeIncreaseSpeed = 1.9;
	const float brakeDecreaseSpeed = 1.7;

	const float turnSpeed = 6.5;

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
