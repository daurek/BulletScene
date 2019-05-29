/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Entity
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

// Header
#include "Entity.h"
// System
#include <iostream>
// Libraries
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model_Obj.hpp"
// Project
#include "Cube.hpp"
#include "Scene.h"

namespace bullet
{
	Entity::Entity(std::string _name, Scene & scene, std::shared_ptr<btCollisionShape> _physicsShape, btVector3 position, std::shared_ptr<glt::Material> material,
		RigidBodyType _type, std::string modelPath, float mass) : physicsShape(_physicsShape), type(_type), name(_name)
	{
		std::cout << "Entity: " << name << std::endl;

		// Check model path
		if (modelPath.empty())
		{
			// If no model path then use default model
			std::cout << "	Default Model" << std::endl;
			model = std::make_shared<glt::Model>();
			model->add(std::make_shared< glt::Cube >(), material);
		}
		else
		{
			// If model path then load it
			std::cout << "	Model: " << modelPath << std::endl;
			model = std::make_shared<glt::Model_Obj>("../../assets/" + modelPath);
			// Error check
			if (!std::dynamic_pointer_cast<glt::Model_Obj>(model)->is_ok())
			{
				std::cout << "	ERROR: " << std::dynamic_pointer_cast<glt::Model_Obj>(model)->get_error() << std::endl;
				return;
			}
		}
		
		// Set transform
		physicsTransform.setIdentity();
		physicsShape->getAabb(physicsTransform, btVector3(), bounds);
		physicsTransform.setOrigin(position);
		std::cout << "	Position: " << position.x() << "  " << position.y() << "  " << position.z() << "  " << std::endl;
		motionState.setWorldTransform(physicsTransform);
		std::cout << "	Bounds: " << bounds.x() << "  " << bounds.y() << "  " << bounds.z() << "  " << std::endl;
		// Set physics mass and inertia
		btVector3 localInertia;
		physicsShape->calculateLocalInertia(mass, localInertia);

		// Create RigidBody with info
		rigidBody  = std::make_shared<btRigidBody>(btRigidBody::btRigidBodyConstructionInfo (mass, &motionState, physicsShape.get(), localInertia));
		rigidBody->setRestitution(0.0f);
		rigidBody->setUserPointer(this);
		SetRigidBodyType(type);

		// Add the Rigidbody to the Physics World 
		scene.physicsWorld->dynamicsWorld->addRigidBody(rigidBody.get());
		// Add the Model to the Render Scene
		scene.renderScene->add(name, model);
	}

	Entity::~Entity()
	{
		// 
	}

	void Entity::Update(float deltaTime)
	{
		// Sync physical and graphical transform
		rigidBody->getMotionState()->getWorldTransform(physicsTransform);
		physicsTransform.getOpenGLMatrix(glm::value_ptr(graphicsTransform));
		model->set_transformation(graphicsTransform);
		model->scale(bounds.x(), bounds.y(), bounds.z());
	}

	void Entity::SetRigidBodyType(RigidBodyType newType)
	{
		// Set given type as collision flags
		switch (newType)
		{
			case bullet::DYNAMIC:
			{
				rigidBody->setCollisionFlags(0);
				break;
			}
			case bullet::KINEMATIC:
			{
				rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
				break;
			}
			case bullet::STATIC:
			{
				rigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
				break;
			}
			default:
				break;
		}
	}

	void Entity::DisableDeactivation()
	{
		// Wake up
		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}

}