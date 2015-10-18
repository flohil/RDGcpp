#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

Game::Game()
{
	LOG(INFO) << "Starting RDG++...";

	// obtain desktop vMode for default settings
	desktopVmode = sf::VideoMode::getDesktopMode();

	// game settings initialization
	settings.reset(new Settings(desktopVmode.width, desktopVmode.height));
	if (!settings->loadedSuccesfully())
	{
		successfullyInitialized = false;
		return;
	}

	resourceManager.setSettings(settings);

	// load resources
	if (!resourceManager.loadAdditionalResources())
	{
		successfullyInitialized = false;
		return;
	}

	// prototype initalization
	prototypeStorage.reset(new PrototypeStorage(settings->CONFIG_PATH));
	if (!prototypeStorage->initializedSuccessfully())
	{
		successfullyInitialized = false;
		return;
	}

	// create video mode and window
	vmode = sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH);
	window.create(vmode, settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

	window.setFramerateLimit(60); //limit fps to 60 

	// retrieve a list of all possible fullscreen video modes
	std::vector<sf::VideoMode> vmodes = sf::VideoMode::getFullscreenModes();

	// create and print all game objects for test purposes
	// prototypeStorage->testPrintGameObjects();

	background.setTexture(resourceManager.getTexture("tileset"));
	background.setTextureRect(sf::IntRect(1*32,0,32,32));
}

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

void Game::gameLoop()
{
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float deltaTime = elapsed.asSeconds();

		if (peekState() == nullptr)
		{
			continue;
		}
		window.clear(sf::Color::Black);
		peekState()->draw(deltaTime);
		peekState()->handleInput();
		peekState()->update(deltaTime);
		window.display();
	}
}

Game::~Game()
{
	while (!this->states.empty())
	{
		popState();
	}
}