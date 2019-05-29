/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Scene
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

// Project
#include "Scene.h"
#include "Light.hpp"
#include "Catapult.h"
#include "Platform.h"
#include "LastPlatform.h"
#include "Trigger.h"
#include "Key.h"

namespace bullet
{
	Scene::Scene()
	{
		// Physics World
		physicsWorld = std::make_shared<PhysicsWorld>();

		// Graphics Scene
		renderScene = std::make_shared<glt::Render_Node>();
		renderScene->add("camera", std::make_shared <glt::Camera>(20.f, 0.1f, 100.f, 1.f));
		std::shared_ptr<glt::Light> light = std::make_shared <glt::Light>();
		light->translate({ 10.f, 10.f, 10.f });
		renderScene->add("light", light);
		renderScene->get("camera")->rotate_around_x(-0.7f);
		renderScene->get("camera")->translate({ 0.f, 5.f, 30.f });

		// Create Material copies and set colors
		auto wallMat			= std::make_shared< glt::Material>(*glt::Material::default_material().get());
		auto movingPlatformMat	= std::make_shared< glt::Material>(*glt::Material::default_material().get());
		auto winMat				= std::make_shared< glt::Material>(*glt::Material::default_material().get());
		auto catapultMat		= std::make_shared< glt::Material>(*glt::Material::default_material().get());
		wallMat				->set("material_color", { 0.35f, 0.3f, 0.2f });
		movingPlatformMat	->set("material_color", { 0.35f, 0.55f, 0.2f });
		winMat				->set("material_color", { 0.7f, 0.35f, 0.2f });
		catapultMat			->set("material_color", { 0.35f, 0.35f, 0.7f });

		// Scene foundations
		AddEntity("catapultFloor",		std::make_shared<Platform>("catapultFloor", *this,			std::make_shared<btBoxShape>(btVector3(3.5f, 1.5f, 5.5f)), btVector3(16.5f, 0.f, 0.f),	wallMat, bullet::STATIC));
		AddEntity("triggerPlatform",	std::make_shared<Trigger>("triggerPlatform", *this,			std::make_shared<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)), btVector3(8.f, 1.5f, 0.f),	winMat, bullet::DYNAMIC));
		AddEntity("movingPlatform",		std::make_shared<MovingPlatform>("movingPlatform", *this,	std::make_shared<btBoxShape>(btVector3(2.9f, 0.5f, 3.f)),  btVector3(9.5f, 0.3f, 0.f),	movingPlatformMat, bullet::DYNAMIC));
		AddEntity("doorFloor",			std::make_shared<Platform>("doorFloor", *this,				std::make_shared<btBoxShape>(btVector3(3.5f, 1.2f, 5.5f)), btVector3(0.f, -0.3f, 0.f),	wallMat, bullet::STATIC));
		AddEntity("key",				std::make_shared<Key>("key", *this,							std::make_shared<btBoxShape>(btVector3(0.7f, 0.7f, 0.7f)), btVector3(1.5f, 2.f, 2.5f),	movingPlatformMat, bullet::DYNAMIC, "key.obj"));
		AddEntity("door",				std::make_shared<Door>("door", *this,						std::make_shared<btBoxShape>(btVector3(0.5f, 8.5f, 5.5f)), btVector3(-4.2f, 3.5f, 0.f), movingPlatformMat, bullet::DYNAMIC));
		AddEntity("lastFloor",			std::make_shared<Platform>("lastFloor", *this,				std::make_shared<btBoxShape>(btVector3(3.5f, 1.5f, 5.5f)), btVector3(-16.5f, 0.f, 0.f), wallMat, bullet::STATIC));
		AddEntity("winCondition",		std::make_shared<LastPlatform>("winCondition", *this,		std::make_shared<btBoxShape>(btVector3(1.5f, 3.5f, 1.5f)), btVector3(-16.5f, 3.f, 0.f), winMat, bullet::DYNAMIC, "", 1.0f));
	
		// Catapult Compound chassis (move origin to balance)
		btVector3 catapultDimensions(2.f, btScalar(0.5f), 2.f);
		btCollisionShape* chassisShape = new btBoxShape(catapultDimensions);
		btTransform localTransform;
		localTransform.setIdentity();
		localTransform.setOrigin(btVector3(0.f, 1.f, 0.f));
		std::shared_ptr<btCompoundShape> compound = std::make_shared<btCompoundShape>();
		compound->addChildShape(localTransform, chassisShape);

		// Create Catapult and thrower
		AddEntity("catapultThrower",	std::make_shared<Platform>("catapultThrower", *this,		std::make_shared<btBoxShape>(btVector3(2.5f, 0.2f, 0.5f)), btVector3(17.5f, 6.f, 0.f), catapultMat, bullet::DYNAMIC, "", 10.0f));
		AddEntity("catapult",			std::make_shared<Catapult>("catapult", *this,				compound, btVector3(17.f, 4.f, 0.f), catapultMat, bullet::DYNAMIC, "", 800.0f));

		// Set references
		auto catapult =  std::dynamic_pointer_cast<Catapult>(entityMap["catapult"]);
		catapult->scene = this;
		catapult->dimensions = catapultDimensions;
		std::dynamic_pointer_cast<Trigger>(entityMap["triggerPlatform"])->movingPlatform = std::dynamic_pointer_cast<MovingPlatform>(entityMap["movingPlatform"]);
		std::dynamic_pointer_cast<Key>(entityMap["key"])->door = std::dynamic_pointer_cast<Door>(entityMap["door"]);

		// Disable sleep on some rigidbodies
		entityMap["catapult"]		->DisableDeactivation();
		entityMap["door"]			->DisableDeactivation();
		entityMap["winCondition"]	->DisableDeactivation();
		entityMap["movingPlatform"]	->DisableDeactivation();
		entityMap["catapultThrower"]->DisableDeactivation();

		// Start every entity
		for (auto & entity : entityMap)
			entity.second->Start();

	}

	Scene::~Scene()
	{
		// Clear
	}

	void Scene::Update(btScalar deltaTime)
	{
		// Bullet world step and collision check
		physicsWorld->Step(deltaTime);
		physicsWorld->CollisionCallbacks();

		// Update every entity
		for (auto & entity : entityMap)
			entity.second->Update(deltaTime);
	}

	void Scene::Render()
	{
		// Render the whole scene
		renderScene->render();
	}

	void Scene::ResetViewport(const sf::Window & window)
	{
		GLsizei width = GLsizei(window.getSize().x);
		GLsizei height = GLsizei(window.getSize().y);

		renderScene->get_active_camera()->set_aspect_ratio(float(width) / height);

		glViewport(0, 0, width, height);
	}

	std::shared_ptr<Entity> Scene::AddEntity(std::string name, std::shared_ptr<Entity> entity)
	{
		// Error check
		if (!name.empty())
			entityMap[name] = entity;

		return entity;
	}

	std::shared_ptr<Entity> Scene::GetEntity(std::string name)
	{
		// Error check
		if (name.empty())
			return nullptr; 

		return entityMap[name];
	}
}