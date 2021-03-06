#ifndef GAME_STATE_LOADING
#define GAME_STATE_LOADING

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

	unsigned int triggeredLoadingCountdown = 10;
	float minLoadTime = 2.f; // to allow loading music to be played
	float passedLoadTime = 0.f;
	bool loaded = false;
	std::shared_ptr<GameState> loadedGame;
	bool triggeredGameMusicLoad = false;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	virtual void loadGui();
	void triggerLoading();

	GameStateLoading(Game& game);
};

#endif /* GAME_STATE_LOADING */