/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Key
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef KEY_HEADER 
#define KEY_HEADER 

// Project
#include "Door.h"

namespace bullet
{
	/// Opens the Door after being contacted by the Catapult
	class Key : public Entity
	{

	public:

		using Entity::Entity;

		virtual void Start() override { /* Empty */}

		/// On Contact with the catapult open the gate
		virtual void OnCollision(Entity * otherEntity) override
		{
			// Only if it's the catapult and the key hasnt been taken
			if (otherEntity->getName() == "catapult" && !taken)
			{
				// Toggle flag, move key away and open door
				taken = true;
				rigidBody->translate(btVector3{ 0, 30, 30 });
				rigidBody->activate(true);
				door->Open();
			}
		}

		/// Door reference
		std::shared_ptr<Door> door;

	private:

		/// Key taken flag
		bool taken = false;
	};
}

#endif 

