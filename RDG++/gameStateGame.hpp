#ifndef GAME_STATE_GAME
#define GAME_STATE_GAME

#include <SFML/Graphics.hpp>

#include "gameState.hpp"
#include "map.hpp"
#include "fight.hpp"

class DetailsBag
{
public:

	std::string getName() const { return name; };
	std::vector<std::vector<std::string>> getDetails() const { return details; };
	sf::Texture& getDetailsPic() const { return detailsPic; };

	//consturctor for different item types and key and monsters
	DetailsBag(std::shared_ptr<RenderableObject> obj) : DetailsBag(obj, true) {	};
	DetailsBag(std::shared_ptr<RenderableObject> obj, bool playSound);

private:

	std::string name;
	std::vector<std::vector<std::string>> details;
	sf::Texture& detailsPic;

	void addRow(std::string key, std::string value);
};

class GameStateGame : public GameState
{
private:

	sf::View statsView;
	sf::View mapView;
	sf::View fightView;
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
	sf::Sprite playerSprite;
	sf::Sprite enemySprite;

	sf::RectangleShape fightBackground;

	std::shared_ptr<RenderableObject> draggedItem = nullptr;
	bool dragging = false;
	bool draggedFromEquipment;

	bool mouseDeterminationTriggered = false;
	sf::Vector2i lastClickPos;
	sf::Vector2i dragStartPos;

	bool usePotionActive = false;
	bool inAttackOptions = false;
	bool inFight = false;
	bool choseChangeSet = false;
	bool interactionPermitted = true;
	std::shared_ptr<Fight> fight = nullptr;

	float fightStageSpan = 1.f; // second
	float fightStageAccumulator = 0.f;

	// gui
	float topVerSplit;
	float rightHorSplit;
	float rightVerSplit;
	float bottomVerSplit;
	float bottomHorSplit;
	int topVerSplitAbs;
	int rightHorSplitAbs;
	int rightVerSplitAbs;
	int bottomVerSplitAbs;
	int bottomHorSplitAbs;

	float armorHeight;
	float armorTopOffset;
	float armorLeftOffset;
	float potionTopOffset;
	float potionLeftOffset;

	tgui::Gui statsGui;
	tgui::Gui chatGui;
	tgui::Gui detailsGui;
	tgui::Gui fightGui;
	tgui::Gui inventoryGui;
	tgui::Gui armorGui;

	tgui::Theme::Ptr theme;

	tgui::ChatBox::Ptr statsbox;
	tgui::ChatBox::Ptr chatbox;
	tgui::ChatBox::Ptr detailsbox;
	tgui::ChatBox::Ptr inventorybox;
	tgui::ChatBox::Ptr armorbox;

	tgui::Label::Ptr playerNameLabel;
	tgui::Label::Ptr hpLabel;
	tgui::Label::Ptr accuracyLabel;
	tgui::Label::Ptr strengthLabel;
	tgui::Label::Ptr speedLabel;
	tgui::Label::Ptr playerNameFightLabel;
	tgui::Label::Ptr enemyNameFightLabel;

	sf::Sprite hpSprite;
	sf::Sprite accuracySprite;
	sf::Sprite strengthSprite;
	sf::Sprite speedSprite;

	float statsMarginRight;
	float statsSpacing;
	float statsPicSize;

	tgui::Button::Ptr set1Button;
	tgui::Button::Ptr set2Button;

	tgui::Label::Ptr detailsHeader;
	tgui::Grid::Ptr detailsGrid;
	sf::Sprite detailsSprite;

	float detailsHeaderMiddle;
	float detailsMiddle;
	float detailsKeyMiddle;
	float detailsValueMiddle;
	float detailsLeftAnchor;
	float detailsRightAnchor;
	float detailsPicSize;
	float detailsPicLeft;
	float detailsPicTop;

	std::vector<std::vector<tgui::Label::Ptr>> details;
	unsigned int detailRows;

	tgui::Button::Ptr attackButton;
	tgui::Button::Ptr parryButton;
	tgui::Button::Ptr potionButton;
	tgui::Button::Ptr equipmentSetButton;
	tgui::Button::Ptr headButton;
	tgui::Button::Ptr torsoButton;
	tgui::Button::Ptr armsButton;
	tgui::Button::Ptr legsButton;
	bool pressedAttackMainMenuButton = false;
	bool showingEnemyDetails = false;

	tgui::ProgressBar::Ptr enemyHealthBar;
	tgui::ProgressBar::Ptr playerHealthBar;

	sf::Vector2f initialMapCenter;

	void changeSet() { changeSet(false); };
	void changeSet(bool playSound);
	void changeSet(bool playSound, unsigned int numerator);
	void handleMouseEvent(sf::Vector2i pos_, MouseEvent::Enum eventType) { handleMouseEvent(pos_, eventType, false); };
	void handleMouseEvent(sf::Vector2i pos_, MouseEvent::Enum eventType, bool noScale);
	void updateDetails(DetailsBag& detailsBag) { updateDetails(detailsBag, false); };
	void updateDetails(DetailsBag& detailsBag, bool showingEnemyDetails_);
	void updateStats();
	void startFight(std::shared_ptr<Player> player_, std::shared_ptr<Monster> monster_);
	void endFight(std::shared_ptr<Creature> loser);
	void toggleAttackGui();
	void hideAttackGui();
	void parry();
	void usePotion();
	void toggleEquipment();
	void attackHead();
	void attackTorso();
	void attackArms();
	void attackLegs();

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