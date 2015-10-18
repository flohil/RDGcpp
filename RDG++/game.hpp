#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include "settings.hpp"
#include "prototypes.hpp"
#include "resourceManager.hpp"
#include "maze.hpp"

// forward declaration of GameState to allow including of game.hpp in gameState.hpp and avoid include cycle
class GameState; 

class Game
{
public:

	std::stack<GameState*> states;
	sf::RenderWindow window;
	std::shared_ptr<RenderableObject> background;
	sf::Sprite backgroundSprite;

	bool initializedSuccessfully() const { return successfullyInitialized; };
	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();
	std::shared_ptr<Settings> getSettings() const { return settings; };
	std::shared_ptr<PrototypeStorage> getPrototypeStorage() const { return prototypeStorage; };
	ResourceManager& getResourceManager() const { return resourceManager; };
	std::vector<sf::VideoMode> getVModes() const { return vmodes; };

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

};

#endif /* GAME_HPP */