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

	resourceManager.setSoundVolumes(settings->actualEffectsVolume);
	music.setVolume(settings->actualMusicVolume);
	music.setLoop(true);
	currentMusic = "mainMenu";

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

			if (musicInChange) // fade out previous music
			{
				changeMusic(deltaTime);
			}
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

void Game::changeMusic(const float deltaTime)
{
	if (!finishedFade)
	{
		fadeOutAccumulator += deltaTime;

		float currentVol = music.getVolume();
		float maxVol = settings->actualMusicVolume;

		float newVol = (fadeOutSpan - fadeOutAccumulator) / fadeOutSpan * maxVol;

		if (fadeOutAccumulator >= fadeOutSpan)
		{
			fadeOutSpan = 0.f;
			fadeOutAccumulator = 0.f;
			newVol = 0.f;
			finishedFade = true;
			ResourceManager::getInstance().getMusic(currentMusic)->timeOffset = music.getPlayingOffset();
			music.pause();
		}

		music.setVolume(newVol);
	}
	if (finishedFade && !finishedPause)
	{
		pauseAccumulator += deltaTime;

		if (pauseAccumulator >= pauseSpan)
		{
			finishedPause = true;
			prevMusic = currentMusic;
			currentMusic = nextMusic;
			music.openFromFile(ResourceManager::getInstance().getMusic(currentMusic)->path);
			music.play();
			if (fromPrevOffset)
			{
				music.setPlayingOffset(ResourceManager::getInstance().getMusic(currentMusic)->timeOffset);
			}
			else
			{
				music.setPlayingOffset(sf::seconds(0));
			}
			nextMusic.clear();
			musicReady = true;
		}
	}
	if (finishedFade && finishedPause)
	{
		fadeInAccumulator += deltaTime;

		//std::cout << "fadeInAcc: " << fadeInAccumulator << ", fadeInSpan: " << fadeInSpan << std::endl;

		float currentVol = music.getVolume();
		float maxVol = settings->actualMusicVolume;

		float newVol = fadeInAccumulator / fadeInSpan * maxVol;

		if (fadeInAccumulator >= fadeInSpan)
		{
			fadeInSpan = 0.f;
			fadeInAccumulator = 0.f;
			newVol = maxVol;
			musicInChange = false;
		}

		music.setVolume(newVol);
	}
}

void Game::changeMusic(std::string nextMusic_, const float fadeOutSpan_, const float fadeInSpan_, const float pauseSpan_, bool fromPrevOffset_)
{
	std::cout << "changeMusic set parameters for " << nextMusic_ << std::endl;
	std::cout << "fadeOut: " << fadeOutSpan_ << ", fadeIn: " << fadeInSpan_ << ", pause: " << pauseSpan_ << std::endl;

	nextMusic = nextMusic_;
	fadeOutSpan = fadeOutSpan_;
	fadeInSpan = fadeInSpan_;
	pauseSpan = pauseSpan_;
	fadeOutAccumulator = 0.f;
	fadeInAccumulator = 0.f;
	pauseAccumulator = 0.f;
	finishedFade = false;
	finishedPause = false;
	fromPrevOffset = fromPrevOffset_;
	musicInChange = true;
	musicReady = false;
}

Game::~Game()
{
	while (!states.empty())
	{
		popState();
	}
}