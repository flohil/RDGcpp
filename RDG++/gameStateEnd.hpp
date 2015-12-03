#ifndef GAME_STATE_VICTORY
#define GAME_STATE_VICTORY

#include <SFML/Graphics.hpp>

#include "gameState.hpp"

class GameStateEnd : public GameState
{
private:

	sf::View view;
	sf::Sprite background;
	tgui::Gui gui;

	// gui measures
	bool victory;

	bool triggeredQuit = false;
	float quitSpan = 1.0f;
	float quitAccumulator = 0.f;

	float scrollUpdateSpan = 0.05f;
	float scrollUpdateAccumulator = 0;
	float totalAccumulator = 0;
	float totalDuration;

	sf::Text endCaptionText;
	sf::Text text;
	sf::Font font;
	std::vector<sf::Text> lines;

	float endPosY;
	float targetEndPosY;
	float victoryDuration = 140.f; // in seconds
	float defeatDuration = 121.f; // in seconds
	float staticEndScreenDuration = 0.f; // in seconds
	float scrollSpeed = 0.f;

	bool endedScrolling = false;
	unsigned int scrollCtr = 0;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	virtual void loadGui();

	GameStateEnd(Game& game, bool victory);
};

#endif /* GAME_STATE_VICTORY */