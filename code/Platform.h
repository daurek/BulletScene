/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Platform
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef PLATFORM_HEADER 
#define PLATFORM_HEADER 

// Project
#include "Entity.h"

namespace bullet
{
	/// Basic Platform with no logic
	class Platform : public Entity
	{

	public:

		using Entity::Entity;

		virtual void Start() override { /* Empty */ }

		virtual void OnCollision(Entity * otherEntity) override { /* Empty */ }
	};
}

#endif 

