/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Scene
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef SCENE_HEADER
#define SCENE_HEADER 

// Libraries
#include <btBulletDynamicsCommon.h>
#include "Render_Node.hpp"
#include "SFML/Window.hpp"
// Project
#include "PhysicsWorld.h"
#include "Entity.h"

namespace bullet
{
	/// Container Scene that render the OpenGL context and creates a physical world with Entities
	class Scene
	{

	public:

		/// Creates and configurates physical world and graphical scene
		Scene();

		~Scene();

		/// Handles physical world step and collisions along with Updates on every Entity
		void Update(btScalar deltaTime);

		/// Renders the graphical scene
		void Render();

		/// Reset viewport on resize
		void ResetViewport(const sf::Window & window);

		/// Add entity to the map
		std::shared_ptr<Entity> AddEntity(std::string name, std::shared_ptr<Entity> entity);

		/// Returns entity from the map
		std::shared_ptr<Entity> GetEntity(std::string name);
		
		/// Physical world Pointer
		std::shared_ptr< PhysicsWorld > physicsWorld;

		/// Render scene Pointer
		std::shared_ptr< glt::Render_Node > renderScene;

	private:
		
		/// Entities map
		std::map<std::string, std::shared_ptr<Entity>> entityMap;
	};
}

#endif 

