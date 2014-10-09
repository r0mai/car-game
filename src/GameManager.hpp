
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

	GameManager(const CommonParameters& parameters, std::shared_ptr<const track::Track> track);

	void advance();

	void setNeuralNetwork(const NeuralNetwork& network);

	void init();

	bool getIsAIControl() const { return isAIControl; }
	void setIsAIControl(bool value) { isAIControl = value; }
	Model& getModel() { return model; }
	const Model& getModel() const { return model; }
	const std::shared_ptr<const track::Track>& getTrack() const { return track; }
	std::vector<boost::optional<sf::Vector2f>> getRayPoints() {
		return rayPoints;
	}
	const CommonParameters& getCommonParameters() { return parameters; }
private:
	void controlCar();

	Weights callNeuralNetwork();

	CommonParameters parameters;

	float physicsTimeStep;

	std::vector<boost::optional<sf::Vector2f>> rayPoints;

	NeuralNetwork neuralNetwork;

	bool isAIControl = true;

	Model model;
	std::shared_ptr<const track::Track> track;
};

}

#endif

