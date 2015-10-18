#ifndef GAME_STATE_GAME
#define GAME_STATE_GAME

#include <SFML/Graphics.hpp>

#include "gameState.hpp"
#include "map.hpp"

class GameStateGame : public GameState
{
private:

	sf::View view;
	sf::View guiView;
	std::unique_ptr<Map> map;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();

	GameStateGame(Game& game);
};

#endif /* GAME_STATE_GAME */