/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::PhysicsWorld
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef PHYSICS_WORLD_HEADER 
#define PHYSICS_WORLD_HEADER 

// System
#include <memory>
// Libraries
#include <btBulletDynamicsCommon.h>

namespace bullet
{
	/// Bullet world container that handles physical steps and collision callbacks
	class PhysicsWorld
	{

	public:

		/// Creates physical world with given gravity
		PhysicsWorld(btVector3 worldGravity = { 0.f, -10.f, 0.f });

		~PhysicsWorld();

		/// World Physical step
		void Step(btScalar stepValue);

		/// Collision check that callbacks to both collision Entities
		void CollisionCallbacks();

		/// Resets the physical world
		void ResetWorld();

		/// Physics World pointer
		std::shared_ptr< btDiscreteDynamicsWorld > dynamicsWorld;

	private:

		/// Collision configuration
		btDefaultCollisionConfiguration collisionConfiguration;

		/// Default collision dispatcher
		btCollisionDispatcher collisionDispatcher;

		/// Broadphase
		btDbvtBroadphase overlappingPairCache;

		/// Default constraint solver
		btSequentialImpulseConstraintSolver constraintSolver;
	};
}

#endif 

