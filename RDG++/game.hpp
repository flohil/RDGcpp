#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>

// forward declaration of GameState to allow including of game.hpp in gameState.hpp and avoid include cycle
class GameState; 

class Game
{
public:

	std::stack<GameState*> states;
	sf::RenderWindow window;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	Game();
	~Game();
};

#endif /* GAME_HPP */