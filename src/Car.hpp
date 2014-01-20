#ifndef CAR_HPP
#define CAR_HPP

#include <irrlicht/irrlicht.h>

namespace car {

class Car {
public:

	typedef irr::core::vector3df Vector3d;

	Car() = default;
	Car(const Vector3d& position);

	void setMesh(irr::scene::IMeshSceneNode *newMesh);

	void move(irr::f32 deltaSeconds);

	const Vector3d& getPosition() const;
	const Vector3d& getVelocity() const;
	irr::f32 getSpeed() const;

private:
	void updateMesh();

	Vector3d position;

	Vector3d velocity = Vector3d(10, 0, 0);
	const irr::f32 mass = 1000.0; //1000kg

	irr::scene::IMeshSceneNode *mesh;
};

}

#endif /* !CAR_HPP */
