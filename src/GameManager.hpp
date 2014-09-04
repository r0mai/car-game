
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <vector>
#include <functional>

#include "Model.hpp"
#include "CommonParameters.hpp"
#include "NeuralNetwork.hpp"
#include "Track/TrackCreator.hpp"

namespace car {

class GameManager {
public:

	GameManager(const CommonParameters& parameters, track::TrackCreator trackCreator);

	void advance();

	void setNeuralNetwork(const NeuralNetwork& network);

	void init();

	bool getIsAIControl() const { return isAIControl; }
	void setIsAIControl(bool value) { isAIControl = value; }
	Model& getModel() { return model; }
	const Model& getModel() const { return model; }
	const track::Track& getTrack() const { return track; }
	std::vector<boost::optional<sf::Vector2f>> getRayPoints() {
		return rayPoints;
	}
private:
	void controlCar();

	Weights callNeuralNetwork();

	CommonParameters parameters;

	float physicsTimeStep;

	// We have to store rayCount here as well, because with setNeuralNetwork it
	// is possible to set a neuralNetwork which is not consistent with what is
	// specified in parameters TODO something has to be done about who stores
	// the variables concerning the number of inputs/outputs
	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	NeuralNetwork neuralNetwork;

	bool isAIControl = true;

	Model model;
	track::Track track;
};

}

#endif

