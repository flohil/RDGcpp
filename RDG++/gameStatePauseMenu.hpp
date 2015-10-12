#ifndef GAME_STATE_PAUSE_MENU
#define GAME_STATE_PAUSE_MENU

#include <SFML/Graphics.hpp>

#include "gameState.hpp"

class GameStatePauseMenu : public GameState
{
private:

	sf::View view;
	sf::View guiView;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();

	GameStatePauseMenu(Game& game);
};

#endif /* GAME_STATE_PAUSE_MENU */