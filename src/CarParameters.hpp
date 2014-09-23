#ifndef SRC_CARPARAMETERS_HPP
#define SRC_CARPARAMETERS_HPP

namespace car {

struct CarParameters {
	float cDrag = 0.5;
	float cRollingResistance = 14.2;
	float pEngine = 40000.0;
	float fEngineMax = 10000.0;
	float fBrake = 30000.0;
	//float gravity = 9.8; //m/s^2
	//float transMissionEfficiency = 0.7;
	float gearRatio = 2.1;
	float differentialRatio = 3.42;
	float wheelRadius = 0.34; //m
	float mass = 1500; //kg
	float maxTurnAngle = 0.52; //radians ~= 30 degrees
	float turnRate = 8.0;

		//CM == Center of Mass
		//CG == Center of Gravity
	//float rearWheelCMDistance = 1.0;
	//float frontWheelCMDistance = 1.5;
	float rearCMDistance = 1.2;
	float frontCMDistance = 1.8;
	//float heightOfCG = 1.5;
	float carWidth = 1.4;
	float throttleIncreaseSpeed = 1.5;
	float throttleDecreaseSpeed = 1.3;
	float brakeIncreaseSpeed = 1.9;
	float brakeDecreaseSpeed = 1.7;
	float turnSpeed = 6.5;

	float wheelBase() { return rearCMDistance + frontCMDistance; }
};

}


#endif /* SRC_CARPARAMETERS_HPP */
