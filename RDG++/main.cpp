#include <SFML/Graphics.hpp>
#include "SettingsParser.hpp"
#include "pugixml.hpp"
#include "settings.hpp"
#include <iostream>
#include <fstream>

int main()
{

	// game settings variables declaration
	Settings settings;

	// game engine variables declaration
	SettingsParser settingsParser;
	sf::VideoMode desktopVmode;
	sf::VideoMode vmode;
	sf::RenderWindow window;

	// retrieve desktop Vmode
	desktopVmode = sf::VideoMode::getDesktopMode();

	// set default game settings variables
	settings.fullscreen = true;
	settings.width = desktopVmode.width;
	settings.height = desktopVmode.height;

	// load settings from settings file
	if (!settings.loadSettings())
	{
		return -1;
	}

	// create video mode and window
	vmode = sf::VideoMode(settings.width, settings.height, settings.colorDepth);
	window.create(sf::VideoMode(settings.width, settings.height, settings.colorDepth), settings.APPNAME, (settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
	
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
				settings.fullscreen = !settings.fullscreen;
				window.create(sf::VideoMode(settings.width, settings.height, settings.colorDepth), settings.APPNAME, (settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
				std::cout << "fullscreen? " << settings.fullscreen << " - size: " << window.getSize().x << " x " << window.getSize().y << std::endl;
			
				settings.saveSettings();
			}

			// Clear the whole window before rendering a new frame
			window.clear(sf::Color::Red);

			// End the current frame and display its contents on screen
			window.display();
		}
	}

	return 0;
}