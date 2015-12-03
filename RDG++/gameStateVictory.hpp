#ifndef GAME_STATE_VICTORY
#define GAME_STATE_VICTORY

#include <SFML/Graphics.hpp>

#include "gameState.hpp"

class GameStateLoading : public GameState
{
private:

	sf::View view;
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
	virtual void loadGui();

	GameStateLoading(Game& game);
};

#endif /* GAME_STATE_VICTORY */