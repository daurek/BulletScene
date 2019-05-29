/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Trigger
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef TRIGGER_HEADER 
#define TRIGGER_HEADER 

// Project
#include "MovingPlatform.h"

namespace bullet
{
	/// Moves platform after being contacted by the Catapult
	class Trigger : public Entity
	{

	public:

		using Entity::Entity;

		virtual void Start() override { /* Empty */ }

		/// Activates platform movement after some time as soon as the Catapult reaches this
		virtual void Update(float deltaTime) override
		{
			Entity::Update(deltaTime);

			// If contact has been made and platform is not moving
			if (contact && !movingPlatform->moving)
			{
				// Add time until cooldown has been reached
				currentTime += deltaTime;
				if (currentTime >= maxTime)
					// Move the platform
					movingPlatform->moving = true;
			}
		}

		/// Catapult triggers the contact
		virtual void OnCollision(Entity * otherEntity) override
		{
			// Check if it's the catapult
			if (otherEntity->getName() == "catapult")
			{
				// Move trigger away
				rigidBody->translate(btVector3(0.f, 30.f, 30.f));
				rigidBody->activate(true);
				contact = true;
			}
		}

		/// Moving platform reference
		std::shared_ptr<MovingPlatform> movingPlatform;
		
	private:

		/// Contact with Catapult flag
		bool contact = false;

		/// Time counter after contact
		float currentTime = 0.f;

		/// Max time
		float maxTime = 3.f;

	};
}

#endif 

