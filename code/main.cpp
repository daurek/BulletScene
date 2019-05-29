#include <OpenGL.hpp>
#include "Scene.h"
#include <SFML/Window.hpp>

int main ()
{
	// Create sfml window
	sf::Window window
	(
		sf::VideoMode(1024, 720),
		"Bullet Scene",
		sf::Style::Default,
		sf::ContextSettings(24, 0, 0, 4, 1, sf::ContextSettings::Core)     
	);

	// Initialize OpenGL
	if (!glt::initialize_opengl_extensions())
		exit(-1);

	// Vsync
	window.setVerticalSyncEnabled(true);

	// Create bullet scene
	bullet::Scene scene;

	// Reset viewport with given window
	scene.ResetViewport(window);

	// Background color
    glClearColor (0.2f, 0.2f, 0.2f, 1.f);

	// App flag
    bool running = true;

	// frame time
	btScalar deltaTime = 1.0f / 60.0f;

    do
    {
		// User input
        sf::Event event;

        while (window.pollEvent (event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    running = false;
                    break;
                }

                case sf::Event::Resized:
                {
					scene.ResetViewport(window);
                    break;
                }
            }
        }

        // Scene Update (physics + logic)
		scene.Update(deltaTime);

        // Clear and Render scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.Render();
        window.display ();
    }
    while (running);

	// Reset the physics world
	scene.physicsWorld->ResetWorld();

    return EXIT_SUCCESS;
}
