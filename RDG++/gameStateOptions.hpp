#ifndef GAME_STATE_OPTIONS
#define GAME_STATE_OPTIONS

#include <SFML/Graphics.hpp>

#include "gameState.hpp"

class GameStateOptions : public GameState
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

	// gui elements
	tgui::EditBox::Ptr playerNameEditbox;
	tgui::ComboBox::Ptr mazeSizeCombobox;
	tgui::CheckBox::Ptr fullscreenCheckbox;
	tgui::ComboBox::Ptr resolutionsCombobox;
	tgui::Label::Ptr statusLabel;

public:

	virtual void draw(const float deltaTime);
	virtual void update(const float deltaTime);
	virtual void handleInput();
	void returnToMainMenu();
	void saveSettings();
	void loadGui();
	static std::string uiToGuiStrCrossPair(unsigned int nbr1, unsigned int nbr2);
	static sf::Vector2u guiStrCrossPairToUi(std::string guiCrossPairStr);

	GameStateOptions(Game& game);
};

#endif /* GAME_STATE_OPTIONS */