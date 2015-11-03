#ifndef GAME_STATE_GAME
#define GAME_STATE_GAME

#include <SFML/Graphics.hpp>

#include "gameState.hpp"
#include "map.hpp"

class GameStateGame : public GameState
{
private:

	sf::View mapView;
	sf::View chatView;
	sf::View inventoryView;
	sf::View armorView;
	sf::View detailsView;
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

	sf::Sprite armorSprite;

	// gui
	float horSplit;
	float rightVerSplit;
	float verSplit;
	float bottomHorSplit;

	float armorHeight;

	tgui::Gui chatGui;
	tgui::Gui detailsGui;
	tgui::Gui fightGui;
	tgui::Gui inventoryGui;
	tgui::Gui armorGui;

	tgui::Theme::Ptr theme;

	tgui::ChatBox::Ptr chatbox;
	tgui::ChatBox::Ptr detailsbox;
	tgui::ChatBox::Ptr inventorybox;
	tgui::ChatBox::Ptr armorbox;

	tgui::Button::Ptr set1Button;
	tgui::Button::Ptr set2Button;

	void changeSet(unsigned int numerator);

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	virtual void loadGui();
	void adaptMapViewport();
	void pauseGame();

	GameStateGame(Game& game);
	GameStateGame::~GameStateGame();
};

#endif /* GAME_STATE_GAME */