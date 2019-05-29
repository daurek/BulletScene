/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::MovingPlatform
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef MOVING_PLATFORM_HEADER 
#define MOVING_PLATFORM_HEADER 

// Project
#include "Entity.h"

namespace bullet
{
	/// Platform that moves when Catapult interacts with Trigger
	class MovingPlatform : public Entity
	{

	public:

		using Entity::Entity;

		virtual void Start() override { /* Empty */ }

		virtual void Update(float deltaTime) override
		{
			Entity::Update(deltaTime);
			
			// Moves only when flag is activated
			if (moving)
				rigidBody->setLinearVelocity({ -0.5f, 0.f, 0.f });
			else
				rigidBody->setLinearVelocity({ 0.f, 0.f, 0.f });
		}

		virtual void OnCollision(Entity * otherEntity) override
		{	
			// Stops after hitting the next floor
			if (otherEntity->getName() == "doorFloor")
				moving = false;
		}
		
		/// Moving flag
		bool moving = false;

	};
}

#endif 

