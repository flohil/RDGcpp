#ifndef GAME_STATE_MAIN_MENU
#define GAME_STATE_MAIN_MENU

#include <SFML/Graphics.hpp>

#include "gameState.hpp"

class GameStateMainMenu : public GameState
{
private:

	sf::View view;
	sf::View guiView;
	sf::Sprite background;
	tgui::Gui gui;

	// gui measures
	float layoutWidth;
	float layoutHeight;
	float layoutCenterX;
	float layoutCenterY;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	void loadGui();
	void startGame();
	void quit();
	void openSettings();

	GameStateMainMenu(Game& game);
};

#endif /* GAME_STATE_MAIN_MENU */