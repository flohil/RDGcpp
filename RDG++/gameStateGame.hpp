#ifndef GAME_STATE_GAME
#define GAME_STATE_GAME

#include <SFML/Graphics.hpp>

#include "gameState.hpp"
#include "map.hpp"

class DetailsBag
{
public:

	std::string getName() const { return name; };
	std::vector<std::vector<std::string>> getDetails() const { return details; };

	//consturctor for different item types and key and monsters
	DetailsBag(std::shared_ptr<RenderableObject> obj);

private:

	std::string name;
	std::vector<std::vector<std::string>> details;
};

class GameStateGame : public GameState
{
private:

	sf::View mapView;
	sf::View chatView;
	sf::View inventoryView;
	sf::View armorView;
	sf::View detailsView;
	sf::View completeView; // for dragging
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
	sf::Sprite potionSprite;

	std::shared_ptr<RenderableObject> draggedItem = nullptr;
	bool dragging = false;
	bool draggedFromEquipment;

	bool mouseDeterminationTriggered = false;
	sf::Vector2i lastClickPos;
	sf::Vector2i dragStartPos;

	// gui
	float horSplit;
	float rightVerSplit;
	float verSplit;
	float bottomHorSplit;
	int horSplitAbs;
	int rightVerSplitAbs;
	int verSplitAbs;
	int bottomHorSplitAbs;

	float armorHeight;
	float armorTopOffset;
	float armorLeftOffset;
	float potionTopOffset;
	float potionLeftOffset;

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

	tgui::Label::Ptr detailsHeader;
	tgui::Grid::Ptr detailsGrid;

	float detailsMiddle;
	float detailsKeyMiddle;
	float detailsValueMiddle;

	std::vector<std::vector<tgui::Label::Ptr>> details;

	void changeSet(unsigned int numerator);
	void handleMouseEvent(sf::Vector2i pos_, MouseEvent::Enum eventType);
	void updateDetails(DetailsBag& detailsBag);

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