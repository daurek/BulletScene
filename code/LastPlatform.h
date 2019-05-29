/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::LastPlatform
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef LAST_PLATFORM_HEADER 
#define LAST_PLATFORM_HEADER

#include "Entity.h"
#include <iostream>

namespace bullet
{
	/// Wins after hitting it with a projectile
	class LastPlatform : public Entity
	{

	public:

		using Entity::Entity;

		virtual void Start() override { /* Empty */ }

		virtual void OnCollision(Entity * otherEntity) override
		{
			// If hit with a ball 
			if (otherEntity->getName() == "ball" && !hit)
			{
				// Push rigidbody
				hit = true;
				rigidBody->applyForce(btVector3(-200.f, 0.f, 0.f), otherEntity->getRigidBody()->getWorldTransform().getOrigin());
				std::cout << "WIN";
			}
		}

	private:

		/// Hit flag
		bool hit = false;
	};
}

#endif 

