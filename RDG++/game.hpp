#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <TGUI/TGUI.hpp>
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