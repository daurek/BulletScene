/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::PhysicsWorld
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

// Header
#include "PhysicsWorld.h"
// Project
#include "Entity.h"

namespace bullet
{
	PhysicsWorld::PhysicsWorld(btVector3 worldGravity) : collisionDispatcher(&collisionConfiguration)
	{
		// Creates physical world with given gravity and properties
		dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(&collisionDispatcher, &overlappingPairCache, &constraintSolver, &collisionConfiguration);
		dynamicsWorld->setGravity(worldGravity);
	}

	PhysicsWorld::~PhysicsWorld()
	{
		// Clear
	}

	void PhysicsWorld::Step(btScalar stepValue)
	{
		// Physics step
		dynamicsWorld->stepSimulation(stepValue);
	}

	void PhysicsWorld::CollisionCallbacks()
	{
		// Get number of collisions
		int collisions = collisionDispatcher.getNumManifolds();

		// Loop through them
		for (int i = 0; i < collisions; i++)
		{
			// Cache collision
			btPersistentManifold * collision = collisionDispatcher.getManifoldByIndexInternal(i);

			// Check validity
			if (collision != nullptr)
			{
				// Get both objects
				btCollisionObject * collisionObj0 = (btCollisionObject *)(collision->getBody0());
				btCollisionObject * collisionObj1 = (btCollisionObject *)(collision->getBody1());

				// Check validity
				if (collisionObj0 != nullptr && collisionObj1 != nullptr)
				{
					// Get both entities
					Entity * entity0 = (Entity *)collisionObj0->getUserPointer();
					Entity * entity1 = (Entity *)collisionObj1->getUserPointer();

					// Check validity
					if (entity0 != nullptr && entity1 != nullptr)
					{
						// Callbacks to both entities with each other
						entity0->OnCollision(entity1);
						entity1->OnCollision(entity0);
					}
					
				}
			}
		}
	}

	void PhysicsWorld::ResetWorld()
	{
		// Reset world
		dynamicsWorld.reset();
	}
}