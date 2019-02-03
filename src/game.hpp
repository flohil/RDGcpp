#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>
#include "settings.hpp"
#include "prototypes.hpp"
#include "resourceManager.hpp"
#include "maze.hpp"

// forward declaration of GameState to allow including of game.hpp in gameState.hpp and avoid include cycle
class GameState; 

class Game
{
public:

	std::stack<std::shared_ptr<GameState>> states;
	sf::RenderWindow window;
	std::shared_ptr<tgui::Theme> theme;
	std::shared_ptr<RenderableObject> background;
	std::string currentMusic;
	sf::Music music;
	std::string prevMusic;

	bool initializedSuccessfully() const { return successfullyInitialized; };
	void pushState(std::shared_ptr<GameState> state);
	void popState();
	void changeState(std::shared_ptr<GameState> state);
	std::shared_ptr<GameState> peekState();

	void gameLoop();
	std::shared_ptr<Settings> getSettings() const { return settings; };
	std::shared_ptr<PrototypeStorage> getPrototypeStorage() const { return prototypeStorage; };
	ResourceManager& getResourceManager() const { return resourceManager; };
	void reloadGuis();
	void changeMusic(std::string nextMusic_, const float fadeOutSpan_, const float fadeInSpan_, const float pauseSpan_) {
		changeMusic(nextMusic_, fadeOutSpan_, fadeInSpan_, pauseSpan_, false);
	}; //in seconds
	void changeMusic(std::string nextMusic_, const float fadeOutSpan_, const float fadeInSpan_, const float pauseSpan_, bool fromPrevOffset);
	// when pause has finished
	bool isMusicReady() const { return musicInChange; };
		
	Game();
	~Game();

private:

	bool successfullyInitialized = true;
	std::shared_ptr<Settings> settings;
	std::shared_ptr<PrototypeStorage> prototypeStorage;
	ResourceManager& resourceManager = ResourceManager::getInstance();
	std::vector<sf::VideoMode> vmodes;
	sf::VideoMode desktopVmode;
	sf::VideoMode vmode;
	std::unique_ptr<Maze> maze;
	std::string nextMusic;
	float fadeOutSpan;
	float fadeInSpan;
	float pauseSpan;
	float fadeOutAccumulator;
	float fadeInAccumulator;
	float pauseAccumulator;
	bool musicInChange = false;
	bool finishedFade = false;
	bool finishedPause = false;
	bool fromPrevOffset = false;
	bool musicReady = true;
	sf::Time prevMusicOffset;

	void changeMusic(const float deltaTime);
};

#endif /* GAME_HPP */