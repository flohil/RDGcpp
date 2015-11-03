#include <stack>

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
	window.create(vmode, settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Close));

	window.setFramerateLimit(60); //limit fps to 60 

	// Load the black theme
	// theme = std::make_shared<tgui::Theme>("res/widgets/black.txt");

	// retrieve a list of all possible fullscreen video modes
	std::vector<sf::VideoMode> vmodes = sf::VideoMode::getFullscreenModes();

	// create and print all game objects for test purposes
	// prototypeStorage->testPrintGameObjects();
}

void Game::pushState(std::shared_ptr<GameState> state)
{
	states.push(state);

	return;
}

void Game::popState()
{
	LOG(DEBUG) << "deleting state " << states.top();
	//delete states.top();
	states.pop();

	return;
}

void Game::changeState(std::shared_ptr<GameState> state)
{
	if (!states.empty())
	{
		popState();
	}
	pushState(state);

	return;
}

std::shared_ptr<GameState> Game::peekState()
{
	if (states.empty())
	{
		return nullptr;
	}
	return states.top();
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
		if (window.hasFocus())
		{
			peekState()->handleInput();
			peekState()->update(deltaTime);
		}
		window.display();
	}
}

void Game::reloadGuis()
{
	std::stack<std::shared_ptr<GameState>> oldStates;

	while (!states.empty())
	{
		oldStates.push(peekState());
		peekState()->loadGui();
		popState();
	}
	
	while (!oldStates.empty())
	{
		states.push(oldStates.top());
		oldStates.pop();
	}
}

Game::~Game()
{
	while (!states.empty())
	{
		popState();
	}
}