#ifndef SRC_REALTIMEPARAMETERS_HPP
#define SRC_REALTIMEPARAMETERS_HPP

#include "ScreenDimension.hpp"
#include "CommonParameters.hpp"
#include "CarInputParameters.hpp"

namespace car {

enum class PanMode {
	center, fit
};

struct RealTimeParameters {
	CommonParameters commonParameters;
	CarInputParameters carInputParameters;

	std::string projectRootPath;
	//parameters for rendering
	int fpsLimit = -1;
	unsigned screenWidth = 800;
	unsigned screenHeight = 800;
	float minPixelsPerMeter = 5.f;
	float maxPixelsPerMeter = 10.f;
	PanMode panMode = PanMode::fit;
	ScreenDimension panThreshold{Meters(0)};
};

}


#endif /* SRC_REALTIMEPARAMETERS_HPP */
