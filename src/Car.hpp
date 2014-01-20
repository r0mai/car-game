#ifndef CAR_HPP
#define CAR_HPP

#include <irrlicht/irrlicht.h>

namespace car {

class Car {
public:
	typedef irr::core::vector3df Position;

	Car() = default;
	Car(const Position& position);

	void setMesh(irr::scene::IMeshSceneNode *newMesh);

	void updateMesh();

private:
	Position position;
	irr::scene::IMeshSceneNode *mesh;
};

}

#endif /* !CAR_HPP */
