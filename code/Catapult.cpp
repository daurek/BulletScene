/// ----------------------------------------------------------------------------------------------------------------------
/// BULLET SCENE
/// \class bullet::Catapult
///
/// \author Ilyass Sofi Hlimi
/// \date 29/05/2019
///
/// Contact: ilyassgame@gmail.com
/// ----------------------------------------------------------------------------------------------------------------------

// Header
#include "Catapult.h"

namespace bullet
{
	void Catapult::Start()
	{
		// Catapult Thrower Hinge
		catapultThrowerHinge = std::make_shared<btHingeConstraint>(*getRigidBody().get(), *scene->GetEntity("catapultThrower")->getRigidBody().get(), btVector3(0.f, 4.f, 0.f), btVector3(-3.f, 0.f, 0.f), btVector3(0.f, 0.f, 1.f), btVector3(0.f, 0.f, 0.f));
		catapultThrowerHinge->setMaxMotorImpulse(10.f);
		scene->physicsWorld->dynamicsWorld->addConstraint(catapultThrowerHinge.get());

		// Vehicle Raycast 
		vehicleRayCaster = std::make_shared<btDefaultVehicleRaycaster>(scene->physicsWorld->dynamicsWorld.get());
		btRaycastVehicle::btVehicleTuning tuning;
		raycastVehicle = std::make_shared<btRaycastVehicle>(tuning, getRigidBody().get(), vehicleRayCaster.get());
		scene->physicsWorld->dynamicsWorld->addVehicle(raycastVehicle.get());

		// Vehicle wheels setup
		btVector3 wheelDirection(0.f, -1.f, 0.f);
		btVector3 wheelAxle(0.f, 0.f, -1.f);
		btScalar suspensionRestLength(0.7f);
		btScalar wheelWidth(0.4f);
		btScalar wheelRadius(0.5f);
		btScalar connectionHeight(1.2f);
		btVector3 wheelConnectionPoint(dimensions.x() - wheelRadius, connectionHeight, dimensions.z() - wheelWidth);

		//Add the front wheels
		raycastVehicle->addWheel(wheelConnectionPoint, wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, true);
		raycastVehicle->addWheel(wheelConnectionPoint* btVector3(1.f, 1.f, -1.f), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, true);

		//Add the rear wheels
		raycastVehicle->addWheel(wheelConnectionPoint * btVector3(-1.f, 1.f, 1.f), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, false);
		raycastVehicle->addWheel(wheelConnectionPoint * btVector3(-1.f, 1.f, -1.f), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, false);

		// Setup up each wheel
		for (int i = 0; i < raycastVehicle->getNumWheels(); i++)
		{
			// Get wheel
			btWheelInfo& wheel = raycastVehicle->getWheelInfo(i);

			// Wheel physical properties
			wheel.m_suspensionStiffness = 50.f;
			wheel.m_wheelsDampingCompression = btScalar(0.3f) * 2.f * btSqrt(wheel.m_suspensionStiffness);
			wheel.m_wheelsDampingRelaxation = btScalar(0.5f) * 2.f * btSqrt(wheel.m_suspensionStiffness);
			wheel.m_frictionSlip = btScalar(1.2f);
			wheel.m_rollInfluence = 1.f;

			// Wheel graphics model
			scene->renderScene->add("wheel" + i, std::make_shared<glt::Model_Obj>("../../assets/wheel.obj"));
		}
	}
	
	void Catapult::Update(float deltaTime)
	{
		Entity::Update(deltaTime);
		// Model offset due to compound shape
		model->translate({ 0.f, 1.f, 0.f });

		// Input handling
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			// Forward (left)
			raycastVehicle->applyEngineForce(200.f, 0);
			raycastVehicle->applyEngineForce(200.f, 1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			// Backward (right)
			raycastVehicle->applyEngineForce(-200.f, 0);
			raycastVehicle->applyEngineForce(-200.f, 1);
		}
		else
		{
			// Stop vehicle when nothing is pressed
			raycastVehicle->applyEngineForce(0.f, 0);
			raycastVehicle->applyEngineForce(0.f, 1);
			raycastVehicle->setBrake(10.f, 0);
			raycastVehicle->setBrake(10.f, 1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			// Turn left hand (down)
			raycastVehicle->setSteeringValue(0.3f, 2);
			raycastVehicle->setSteeringValue(0.3f, 3);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			// Turn right hand (up)
			raycastVehicle->setSteeringValue(-0.3f, 2);
			raycastVehicle->setSteeringValue(-0.3f, 3);
		}
		else
		{
			// Stop steering
			raycastVehicle->setSteeringValue(0.f, 2);
			raycastVehicle->setSteeringValue(0.f, 3);
		}

		// Shoot
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !stopShooting)
		{
			stopShooting = true;
			catapultThrowerHinge->enableMotor(true);
			catapultThrowerHinge->setMotorTargetVelocity(-7000.0f);

			// First time shooting 
			if (!ball)
			{
				// Create ball and wake up
				ball = std::dynamic_pointer_cast<Platform>(scene->AddEntity("ball", std::make_shared<Platform>("ball", *scene, std::make_shared<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)), catapultThrowerHinge->getRigidBodyB().getWorldTransform().getOrigin() + btVector3(0.f, 13.f, 0.f), glt::Material::default_material(), bullet::DYNAMIC, "sphere.obj", 0.8f)));
				ball->DisableDeactivation();
			}
			// Other times shooting
			else
			{
				// Clear every velocity and force
				ball->getRigidBody()->clearForces();
				ball->getRigidBody()->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
				ball->getRigidBody()->setAngularVelocity(btVector3(0.f, 0.f, 0.f));
				// Reposition ball
				btTransform transform = ball->getRigidBody()->getCenterOfMassTransform();
				transform.setOrigin(catapultThrowerHinge->getRigidBodyB().getWorldTransform().getOrigin() + btVector3(0.f, 13.f, 0.f));
				ball->getRigidBody()->setCenterOfMassTransform(transform);
			}
		}

		// Reload
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			stopShooting = false;
			catapultThrowerHinge->setMotorTargetVelocity(3000.f);
		}

		// Update wheels transforms to keep up with the vehicle physical wheels
		for (int i = 0; i < raycastVehicle->getNumWheels(); i++)
		{
			// Get wheel
			btWheelInfo& wheel = raycastVehicle->getWheelInfo(i);
			glt::Matrix44 mat;
			// Get physical transform
			wheel.m_worldTransform.getOpenGLMatrix(glm::value_ptr(mat));
			// Rotate and scale correctly
			mat = glt::rotate_around_y(mat, 80.f);
			mat = glt::scale(mat, 0.3f, 0.3f, 0.7f);
			// Set transformation
			scene->renderScene->get("wheel" + i)->set_transformation(mat);
		}

	}
}