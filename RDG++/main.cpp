#include <SFML/Graphics.hpp>
#include "SettingsParser.hpp"
#include "settings.hpp"
#include "prototypes.hpp"
#include "enums.hpp"
#include "gameObjects.hpp"
#include "debugPrint.hpp"
#include "resourceManager.hpp"
#include <iostream>
#include <fstream>
#include <memory>

int main()
{

	// game engine variables declaration
	SettingsParser settingsParser;
	sf::VideoMode desktopVmode;
	sf::VideoMode vmode;
	sf::RenderWindow window;
	ResourceManager& resourceManager = ResourceManager::getInstance();

	// obtain desktop vMode for default settings
	desktopVmode = sf::VideoMode::getDesktopMode();

	// game settings and prototype initialization
	std::unique_ptr<Settings> settings;
	std::shared_ptr<PrototypeStorage> prototypeStorage;
	
	settings.reset(new Settings(desktopVmode.width, desktopVmode.height));
	if (!settings->loadedSuccesfully())
	{
		return -1;
	}

	prototypeStorage.reset(new PrototypeStorage(settings->CONFIG_PATH));
	if (!prototypeStorage->initializedSuccessfully())
	{
		return -1;
	}

	// create video mode and window
	vmode = sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH);
	window.create(vmode, settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
	
	window.setFramerateLimit(60); //limit fps to 60 

	// retrieve a list of all possible fullscreen video modes
	std::vector<sf::VideoMode> vmodes = sf::VideoMode::getFullscreenModes();

	//// create and print all game objects for test purposes
	prototypeStorage->testPrintGameObjects();

	// load resources
	if (!resourceManager.loadResources(settings->IMAGE_PATH, prototypeStorage))
	{
		return -1;
	}
	
	// output possible resolutions
	for (unsigned i = 0; i < vmodes.size(); i++) 
	{
		std::clog << vmodes[i].width << " x " << vmodes[i].height << "\n";
	}



	// create a new vertex
	sf::Vertex vertex;

	// set its position
	vertex.position = sf::Vector2f(10, 50);

	// set its color
	vertex.color = sf::Color::Red;

	// set its texture coordinates
	vertex.texCoords = sf::Vector2f(100, 100);

	// create an array of 3 vertices that define a triangle primitive
	sf::VertexArray triangle(sf::Triangles, 3);

	// define the position of the triangle's points
	triangle[0].position = sf::Vector2f(10, 10);
	triangle[1].position = sf::Vector2f(100, 10);
	triangle[2].position = sf::Vector2f(100, 100);

	// define the color of the triangle's points
	triangle[0].color = sf::Color::Red;
	triangle[1].color = sf::Color::Blue;
	triangle[2].color = sf::Color::Green;



	// run the program as long as the window is open
	while (window.isOpen())
	{

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {

				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				settings->fullscreen = !settings->fullscreen;
				window.create(sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH), settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
				std::cout << "fullscreen? " << settings->fullscreen << " - size: " << window.getSize().x << " x " << window.getSize().y << std::endl;
			
				settings->saveSettings();
			}

			// Clear the whole window before rendering a new frame
			window.clear(sf::Color::Black);

			window.draw(triangle);

			// End the current frame and display its contents on screen
			window.display();
		}
	}

	return 0;
}

