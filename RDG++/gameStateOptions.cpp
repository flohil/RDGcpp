#include <SFML/Graphics.hpp>

#include "gameStateOptions.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"
#include <cstdlib>

GameStateOptions::GameStateOptions(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(game.window.getSize());
	guiView.setSize(size);
	view.setSize(size);
	size *= 0.5f; // for positioning view centrally
	guiView.setCenter(size);
	view.setCenter(size);

	status = "";

	background.setTexture(ResourceManager::getInstance().getTexture("background"));

	loadGui();
}

void GameStateOptions::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(background);
	gui.draw();

	return;
}

void GameStateOptions::update(const float deltaTime)
{
	return;
}

void GameStateOptions::handleInput()
{
	sf::Event event;

	while (game.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			game.window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				returnToMainMenu();
			}
		}

		gui.handleEvent(event);
	}
}

void GameStateOptions::returnToMainMenu()
{
	LOG(INFO) << "Returning to main menu...";

	game.popState();

	return;
}

void GameStateOptions::saveSettings()
{
	LOG(INFO) << "Saving settings...";

	bool oldFullScreen = settings->fullscreen;
	unsigned int oldWidth = settings->width;
	unsigned int oldHeight = settings->height;
	
	std::string playerName = playerNameEditbox->getText().toAnsiString();
	std::string selectedItemId = mazeSizeCombobox->getSelectedItemId();
	std::string resolutionStr = resolutionsCombobox->getSelectedItem();
	sf::Vector2u resolution = GameStateOptions::guiStrCrossPairToUi(resolutionStr);
	if (playerName.length() > 0)
	{
		settings->playerName = playerName;
	}
	settings->fullscreen = fullscreenCheckbox->isChecked();
	settings->mazeSize = std::stoul(selectedItemId, nullptr, 0);
	settings->width = resolution.x;
	settings->height = resolution.y;

	settings->saveSettings();

	status = "Settings have been saved!";

	statusLabel->setText(status);

	if (oldFullScreen != settings->fullscreen || oldWidth != settings->width || oldHeight != settings->height)
	{
		game.window.create(sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH), settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

		if (oldWidth != settings->width || oldHeight != settings->height)
		{
			game.reloadGuis();
		}
	}

	return;
}

void GameStateOptions::loadGui()
{
	background.setScale(static_cast<float>(settings->width) / static_cast<float>(background.getTexture()->getSize().x), static_cast<float>(settings->height) / static_cast<float>(background.getTexture()->getSize().y));
	view.setSize(static_cast<float>(settings->width), static_cast<float>(settings->height));
	view.setCenter(static_cast<float>(settings->width) * 0.5f, static_cast<float>(settings->height) * 0.5f);

	gui.removeAllWidgets();
	gui.setWindow(game.window);

	//obtain possible resolutions
	std::vector<sf::VideoMode> availiableVideoModes;
	availiableVideoModes = sf::VideoMode::getFullscreenModes();
	sf::VideoMode desktopVideoMode = sf::VideoMode::getDesktopMode();

	float aspectRatio = static_cast<float>(desktopVideoMode.width) / static_cast<float>(desktopVideoMode.height);

	std::vector<sf::VideoMode>::iterator it = availiableVideoModes.begin();

	while (it != availiableVideoModes.end()) 
	{
		if ((static_cast<float>(it->width) / static_cast<float>(it->height)) != aspectRatio || it->bitsPerPixel != 32u)
		{
			it = availiableVideoModes.erase(it);
		}
		else
		{
			++it;
		}
	}

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	layoutWidth = static_cast<float>(settings->defWidgetWidth);
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	float layoutLeft = layoutCenterX - layoutWidth * 0.5f;
	float horSpace = 20.f;
	float verSpace = 20.f;

	tgui::Label::Ptr playerNameLabel = std::make_shared<tgui::Label>();
	playerNameLabel->setText("Player Name");
	playerNameLabel->setTextColor(sf::Color::Black);
	playerNameLabel->setTextSize(settings->labelSmallTextSize);
	playerNameLabel->setSize(settings->defWidgetWidth - settings->labelPaddingX, settings->labelSmallHeight);

	playerNameEditbox = std::make_shared<tgui::EditBox>();
	playerNameEditbox->setOpacity(0.9f);
	playerNameEditbox->setDefaultText(settings->playerName);
	playerNameEditbox->setTextSize(settings->labelBigTextSize);
	playerNameEditbox->setSize(settings->defWidgetWidth, settings->labelBigHeight);
	playerNameEditbox->setMaximumCharacters(20);

	tgui::Label::Ptr mazeSizeLabel = std::make_shared<tgui::Label>();
	mazeSizeLabel->setText("Maze Size");
	mazeSizeLabel->setTextColor(sf::Color::Black);
	mazeSizeLabel->setTextSize(settings->labelSmallTextSize);
	mazeSizeLabel->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f - settings->labelPaddingX, settings->labelSmallHeight);

	mazeSizeCombobox = std::make_shared<tgui::ComboBox>();
	mazeSizeCombobox->setOpacity(0.9f);
	mazeSizeCombobox->addItem(" 3 x 3", "3");
	mazeSizeCombobox->addItem(" 5 x 5", "5");
	mazeSizeCombobox->addItem(" 7 x 7", "7");
	mazeSizeCombobox->addItem(" 9 x 9", "9");
	mazeSizeCombobox->addItem(" 11 x 11", "11");
	if (settings->mazeSize >= 3 && settings->mazeSize <= 11 && settings->mazeSize % 2 == 1)
	{
		mazeSizeCombobox->setSelectedItemById(std::to_string(settings->mazeSize));
	}
	else
	{
		mazeSizeCombobox->setSelectedItemById(std::to_string(5u));
	}
	mazeSizeCombobox->setTextSize(settings->labelBigTextSize);
	mazeSizeCombobox->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f, settings->labelBigHeight);

	tgui::Label::Ptr fullscreenLabel = std::make_shared<tgui::Label>();
	fullscreenLabel->setText("Fullscreen");
	fullscreenLabel->setTextColor(sf::Color::Black);
	fullscreenLabel->setTextSize(settings->labelSmallTextSize);
	fullscreenLabel->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f - settings->labelPaddingX, settings->labelSmallHeight);

	fullscreenCheckbox = std::make_shared<tgui::CheckBox>();
	fullscreenCheckbox->setOpacity(0.9f);
	fullscreenCheckbox->setSize(settings->labelBigHeight, settings->labelBigHeight);
	if (settings->fullscreen)
	{
		fullscreenCheckbox->check();
	}

	tgui::Label::Ptr resolutionLabel = std::make_shared<tgui::Label>();
	resolutionLabel->setText("Resolution");
	resolutionLabel->setTextColor(sf::Color::Black);
	resolutionLabel->setTextSize(settings->labelSmallTextSize);
	resolutionLabel->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f - settings->labelPaddingX, settings->labelSmallHeight);

	resolutionsCombobox = std::make_shared<tgui::ComboBox>();
	resolutionsCombobox->setOpacity(0.9f);
	for (sf::VideoMode vMode : availiableVideoModes)
	{
		resolutionsCombobox->addItem(GameStateOptions::uiToGuiStrCrossPair(vMode.width, vMode.height));
	}
	resolutionsCombobox->setSelectedItem(GameStateOptions::uiToGuiStrCrossPair(settings->width, settings->height));
	resolutionsCombobox->setTextSize(settings->labelBigTextSize);
	resolutionsCombobox->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f, settings->labelBigHeight);

	statusLabel = std::make_shared<tgui::Label>();
	statusLabel->setText(status);
	statusLabel->setTextColor(sf::Color::Black);
	statusLabel->setTextSize(settings->labelSmallTextSize);
	statusLabel->setSize(settings->defWidgetWidth, settings->labelSmallHeight);

	tgui::Button::Ptr saveButton = std::make_shared<tgui::Button>();
	saveButton->setText("Save");
	saveButton->setOpacity(0.9f);
	saveButton->setTextSize(settings->buttonTextSize);
	saveButton->connect("pressed", [&](){ saveSettings(); });
	saveButton->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f, static_cast<float>(settings->widgetHeight));

	tgui::Button::Ptr backButton = std::make_shared<tgui::Button>();
	backButton->setText("Back");
	backButton->setOpacity(0.9f);
	backButton->setTextSize(settings->buttonTextSize);
	backButton->connect("pressed", [&](){ returnToMainMenu(); });
	backButton->setSize(settings->defWidgetWidth * 0.5f - horSpace * 0.5f, static_cast<float>(settings->widgetHeight));

	// obtain size of all elements
	layoutHeight = playerNameLabel->getSize().y + playerNameEditbox->getSize().y + mazeSizeCombobox->getSize().y + fullscreenCheckbox->getSize().y + resolutionsCombobox->getSize().y + statusLabel->getSize().y + saveButton->getSize().y + 6 * verSpace;
	float layoutTop = layoutCenterY - layoutHeight * 0.5f;
	float heightSum = layoutTop;

	//set positions
	playerNameLabel->setPosition(layoutLeft + settings->labelPaddingX, layoutTop);
	heightSum += playerNameLabel->getSize().y;
	playerNameEditbox->setPosition(layoutLeft, heightSum);
	heightSum += playerNameEditbox->getSize().y;
	heightSum += verSpace; //space
	mazeSizeLabel->setPosition(layoutLeft + settings->labelPaddingX, heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	mazeSizeCombobox->setPosition(layoutLeft + settings->defWidgetWidth * 0.5f + horSpace * 0.5f, heightSum);
	heightSum += mazeSizeCombobox->getSize().y;
	heightSum += verSpace; //space
	fullscreenLabel->setPosition(layoutLeft + settings->labelPaddingX, heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	fullscreenCheckbox->setPosition(layoutLeft + settings->defWidgetWidth * 0.5f + horSpace * 0.5f - 1, heightSum);
	heightSum += mazeSizeCombobox->getSize().y;
	heightSum += verSpace; //space
	resolutionLabel->setPosition(layoutLeft + settings->labelPaddingX, heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	resolutionsCombobox->setPosition(layoutLeft + settings->defWidgetWidth * 0.5f + horSpace * 0.5f, heightSum);
	heightSum += mazeSizeCombobox->getSize().y;
	heightSum += verSpace; //space
	statusLabel->setPosition(layoutLeft + 75, heightSum);
	heightSum += verSpace; //space
	heightSum += verSpace; //space
	saveButton->setPosition(layoutLeft, heightSum);
	backButton->setPosition(layoutLeft + settings->defWidgetWidth * 0.5f + horSpace * 0.5f, heightSum);

	gui.add(playerNameLabel);
	gui.add(playerNameEditbox);
	gui.add(mazeSizeLabel);
	gui.add(mazeSizeCombobox);
	gui.add(fullscreenLabel);
	gui.add(fullscreenCheckbox);
	gui.add(resolutionLabel);
	gui.add(resolutionsCombobox);
	gui.add(statusLabel);
	gui.add(saveButton);
	gui.add(backButton);
}

std::string GameStateOptions::uiToGuiStrCrossPair(unsigned int nbr1, unsigned int nbr2)
{
	std::string retStr = " ";
	retStr = retStr + std::to_string(nbr1) + " x " + std::to_string(nbr2);
	return retStr;
}

sf::Vector2u GameStateOptions::guiStrCrossPairToUi(std::string guiCrossPairStr)
{
	unsigned int crossPos = guiCrossPairStr.find("x");
	unsigned int nbr1Start = 1u;
	unsigned int nbr1Length = crossPos - 2u;
	unsigned int nbr2Start = crossPos + 2u;
	unsigned int nbr2Length = guiCrossPairStr.length() - (crossPos + 2u);

	std::string nbr1 = guiCrossPairStr.substr(nbr1Start, nbr1Length);
	std::string nbr2 = guiCrossPairStr.substr(nbr2Start, nbr2Length);
	char *succ1;
	char *succ2;

	return sf::Vector2u(std::strtoul(nbr1.c_str(), &succ1, 10), std::strtoul(nbr2.c_str(), &succ2, 10));
}