/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Catapult
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef CATAPULT_HEADER 
#define CATAPULT_HEADER 

// System
#include <iostream>
// Libraries
#include "SFML/Window/Keyboard.hpp"
#include "Model_Obj.hpp"
// Project
#include "Entity.h"
#include "Platform.h"
#include "Scene.h"

namespace bullet
{
	/// Catapult handled by the Player able to move by RaycastVehicle (motor) and shoot
	class Catapult : public Entity
	{

	public:

		using Entity::Entity;

		/// Initial Catapult configuration (Raycast Vehicle, hinge constraint and wheels)
		virtual void Start() override;

		/// Vehicle and Thrower Input Handling, Updates the vehicle wheel model transformations
		virtual void Update(float deltaTime) override;
		
		virtual void OnCollision(Entity * otherEntity) override { /* Empty */ }

		/// Scene reference to create projectile and hinge
		Scene *								scene;
		/// Dimensions needed to create wheels 
		btVector3							dimensions;

	private:

		/// Pointer to the vehicle
		std::shared_ptr<btRaycastVehicle>	raycastVehicle;

		/// Pointer to the raycaster used by the vehicle
		std::shared_ptr<btVehicleRaycaster> vehicleRayCaster;

		/// Hinge that joins the vehicle and the thrower
		std::shared_ptr<btHingeConstraint>	catapultThrowerHinge;

		/// Ball reference used to reposition before throwing
		std::shared_ptr<Platform>			ball;

		/// Reload flag
		bool stopShooting =					false;
		
	};
}

#endif 

