#ifndef GAME_STATE_PAUSE_MENU
#define GAME_STATE_PAUSE_MENU

#include <SFML/Graphics.hpp>

#include "gameState.hpp"

class GameStatePauseMenu : public GameState
{
private:

	sf::View view;
	sf::View guiView;
	sf::Sprite background;
	tgui::Gui gui;

	bool quit = false;

	// gui measures
	float layoutWidth;
	float layoutHeight;
	float layoutCenterX;
	float layoutCenterY;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	void returnToGame();
	void quitToMainMenu();
	void loadGui();

	GameStatePauseMenu(Game& game);
};

#endif /* GAME_STATE_PAUSE_MENU */