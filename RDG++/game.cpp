#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

void Game::pushState(GameState* state)
{
	this->states.push(state);

	return;
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();

	return;
}

void Game::changeState(GameState* state)
{
	if (!this->states.empty())
	{
		popState();
	}
	pushState(state);

	return;
}

GameState* Game::peekState()
{
	if (this->states.empty())
	{
		return nullptr;
	}
	return this->states.top();
}

bool Game::gameLoop()
{
	sf::Clock clock;

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


	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float deltaTime = elapsed.asSeconds();

		/*if (peekState() == nullptr)
		{
			continue;
		}
		peekState()->handleInput();
		peekState()->update(deltaTime);*/

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

				settings->saveSettings();
			}

			// Clear the whole window before rendering a new frame
			window.clear(sf::Color::Black);

			//peekState()->draw(deltaTime);
			window.draw(triangle);

			// End the current frame and display its contents on screen
			window.display();
		}
	}

	return true;
}

Game::Game()
{
	LOG(INFO) << "Starting RDG++...";

	// obtain desktop vMode for default settings
	desktopVmode = sf::VideoMode::getDesktopMode();

	// game settings and prototype initialization
	settings.reset(new Settings(desktopVmode.width, desktopVmode.height));
	if (!settings->loadedSuccesfully())
	{
		successfullyInitialized = false;
	}

	prototypeStorage.reset(new PrototypeStorage(settings->CONFIG_PATH));
	if (!prototypeStorage->initializedSuccessfully())
	{
		successfullyInitialized = false;
	}

	// create video mode and window
	vmode = sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH);
	window.create(vmode, settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

	window.setFramerateLimit(60); //limit fps to 60 

	// retrieve a list of all possible fullscreen video modes
	std::vector<sf::VideoMode> vmodes = sf::VideoMode::getFullscreenModes();

	// create and print all game objects for test purposes
	// prototypeStorage->testPrintGameObjects();

	// load resources
	if (!resourceManager.loadResources(settings->IMAGE_PATH, prototypeStorage))
	{
		successfullyInitialized = false;
	}
}

Game::~Game()
{
	while (!this->states.empty())
	{
		popState();
	}
}