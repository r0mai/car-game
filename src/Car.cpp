
#include "Car.hpp"

namespace car {

Car::Car(const Position& position) : position(position) {}

void Car::setMesh(irr::scene::IMeshSceneNode *newMesh) {
	mesh = newMesh;
}

void Car::updateMesh() {
	mesh->setPosition(position);
}

}

