#ifndef GAME_STATE_GAME
#define GAME_STATE_GAME

#include <SFML/Graphics.hpp>

#include "gameState.hpp"
#include "map.hpp"

class GameStateGame : public GameState
{
private:

	sf::View mapView;
	sf::View messageView;
	sf::View inventoryView;
	sf::View playerView;
	Map* map;
	std::shared_ptr<Player> player;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();

	GameStateGame(Game& game);
	GameStateGame::~GameStateGame();
};

#endif /* GAME_STATE_GAME */