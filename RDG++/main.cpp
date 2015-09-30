#include <SFML/Graphics.hpp>
#include "SettingsParser.hpp"
#include "settings.hpp"
#include "exceptions.hpp"
#include "prototypes.hpp"
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

	// obtain desktop vMode for default settings
	desktopVmode = sf::VideoMode::getDesktopMode();

	// game settings and prototype initialization
	std::unique_ptr<Settings> settings;
	std::unique_ptr<PrototypeStorage> prototypeStorage;
	try
	{
		settings.reset(new Settings(desktopVmode.width, desktopVmode.height));
		prototypeStorage.reset(new PrototypeStorage(settings->CONFIG_PATH));
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	Armament* testArm = prototypeStorage->armamentFactory->create("Leather Helmet");

	// create video mode and window
	vmode = sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH);
	window.create(vmode, settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
	
	window.setFramerateLimit(60); //limit fps to 60 

	// retrieve a list of all possible fullscreen video modes
	std::vector<sf::VideoMode> vmodes = sf::VideoMode::getFullscreenModes();

	// output possible resolutions
	for (unsigned i = 0; i < vmodes.size(); i++) 
	{
		std::cout << vmodes[i].width << " x " << vmodes[i].height << "\n";
	}

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
			window.clear(sf::Color::Red);

			// End the current frame and display its contents on screen
			window.display();
		}
	}

	return 0;
}