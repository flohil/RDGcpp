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
	sf::Vector2f borderMargin;
	sf::Vector2f maxScroll;
	sf::Vector2f mapSize;
	sf::Vector2f viewportSize;
	float minTop;
	float maxBottom;
	float minLeft;
	float maxRight;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	void adaptMapViewport();
	void pauseGame();

	GameStateGame(Game& game);
	GameStateGame::~GameStateGame();
};

#endif /* GAME_STATE_GAME */