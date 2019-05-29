/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Entity
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

#ifndef ENTITY_HEADER 
#define ENTITY_HEADER 

// Libraries
#include <btBulletDynamicsCommon.h>
#include "Model.hpp"

namespace bullet
{
	class Scene;

	enum RigidBodyType
	{
		DYNAMIC,
		KINEMATIC,
		STATIC
	};

	/// Base class for every Entity with physics, graphics and logic
	class Entity
	{

	public:

		/// Creates an Entity with the given name on the given scene, with the given physics collision shape with it's type and mass 
		/// on a position and an optional model with it's material 
		Entity(std::string _name, Scene & scene, std::shared_ptr< btCollisionShape > _physicsShape, btVector3 position,
			std::shared_ptr<glt::Material> material, RigidBodyType type = DYNAMIC, std::string modelPath = "", float mass = 0.f );

		virtual ~Entity();

		/// Base Start method used for setup and configuration
		virtual void Start() = 0;

		/// Base Update method used to sync physics and graphics transforms, input and logic
		virtual void Update(float deltaTime);

		/// Sets the rigidbody type as the given one
		void SetRigidBodyType(RigidBodyType newType);

		/// Base collision method, is called on both Entities after a collision
		virtual void OnCollision(Entity * otherEntity) = 0;

		/// Disables physics sleep
		void DisableDeactivation();

		/// Returns the Entity's name
		const std::string & getName() { return name; }

		/// Returns the Entity's Rigidbody
		const std::shared_ptr< btRigidBody > getRigidBody() { return rigidBody; };

	protected:

		/// Physics  Bullet Transform 
		btTransform				physicsTransform;
		/// Graphics OpenGL Transform
		glm::mat4				graphicsTransform;
		/// Rigidbody motion State
		btDefaultMotionState	motionState;
		/// Bounds used to scale up graphics model
		btVector3				bounds;
		/// Rigidbody current type
		RigidBodyType			type;
		/// Entity's name
		std::string				name;

		/// Physical Bullet Collision Shape
		std::shared_ptr< btCollisionShape >				physicsShape;
		/// Physical Rigidbody
		std::shared_ptr< btRigidBody >					rigidBody;
		/// Graphics OpenGL Model
		std::shared_ptr< glt::Model >					model;


	};
}

#endif 

