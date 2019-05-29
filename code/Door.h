/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Door
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef DOOR_HEADER 
#define DOOR_HEADER 

// Project
#include "Entity.h"

namespace bullet
{
	/// Door that opens after Key is picked up
	class Door : public Entity
	{

	public:

		using Entity::Entity;

		virtual void Start() override { /* Empty */ }

		/// Moves door when toggled
		virtual void Update(float deltaTime) override
		{
			Entity::Update(deltaTime);

			// If key has been taken
			if (opening)
			{
				// Start counting while moving door down
				currentTime += deltaTime;
				rigidBody->setLinearVelocity({ 0.f, -4.f, 0.f });

				// After some time stop moving the door
				if (currentTime >= maxTime)
				{
					opening = false;
					rigidBody->setLinearVelocity({ 0.f, 0.f, 0.f });
				}
			}
		}

		virtual void OnCollision(Entity * otherEntity) override { /* Empty */ }
		
		/// Toggle open flag
		void Open()
		{
			opening = true;
		}

	private:

		/// Opening door flag
		bool opening = false;

		/// Counter to stop moving
		float currentTime = 0.f;

		/// Maxtime in which to stop counter
		float maxTime = 3.f;
	};
}

#endif 

