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

	float cDrag = 0.5;
	float cRollingResistance = 14.2;
	float fEngine = 2000.0;
	float fBrake = 30000.0;
	float gravity = 9.8; //m/s^2
	float transMissionEfficiency = 0.7;
	float gearRatio = 2.1;
	float differentialRatio = 3.42;
	float wheelRadius = 0.34; //m
	float mass = 1500; //kg
	float maxTurnAngle = 0.52; //radians ~= 30 degrees
	float turnRate = 8.0;

	//CM == Center of Mass
	//CG == Center of Gravity
	float rearWheelCMDistance = 1.0;
	float frontWheelCMDistance = 1.5;
	float rearCMDistance = 1.2;
	float frontCMDistance = 1.8;
	float heightOfCG = 1.5;
	float wheelBase = rearCMDistance + frontCMDistance;
	float carWidth = 1.4;

	float throttleIncreaseSpeed = 1.5;
	float throttleDecreaseSpeed = 1.3;
	float brakeIncreaseSpeed = 1.9;
	float brakeDecreaseSpeed = 1.7;

	float turnSpeed = 6.5;

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
