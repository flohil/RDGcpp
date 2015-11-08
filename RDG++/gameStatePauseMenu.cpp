#include <SFML/Graphics.hpp>

#include "gameStatePauseMenu.hpp"
#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

GameStatePauseMenu::GameStatePauseMenu(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	size = sf::Vector2f(static_cast<float>(settings->scaleWidth), static_cast<float>(settings->scaleHeight));
	guiView.setSize(size);
	view.setSize(size);
	guiView.setCenter(size * 0.5f);
	view.setCenter(size * 0.5f);

	background.setTexture(ResourceManager::getInstance().getTexture("background"));

	// create gui 
	loadGui();
}

void GameStatePauseMenu::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(background);
	gui.draw();

	return;
}

void GameStatePauseMenu::update(const float deltaTime)
{
	return;
}

void GameStatePauseMenu::handleInput()
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
				returnToGame();
			}
		}

		gui.handleEvent(event);
	}
}

void GameStatePauseMenu::returnToGame()
{
	LOG(INFO) << "Returning to game";

	ResourceManager::getInstance().getSound("buttonClick").play();
	game.popState();

	return;
}

void GameStatePauseMenu::quitToMainMenu()
{
	LOG(INFO) << "Quitting to main menu";

	ResourceManager::getInstance().getSound("buttonClick").play();
	if (!quit)
	{
		quit = true;
		game.popState();
		game.popState();
	}
	
	return;
}

void GameStatePauseMenu::loadGui()
{
	background.setScale(static_cast<float>(settings->width) / static_cast<float>(background.getTexture()->getSize().x), static_cast<float>(settings->height) / static_cast<float>(background.getTexture()->getSize().y));
	view.setSize(static_cast<float>(settings->width), static_cast<float>(settings->height));
	view.setCenter(static_cast<float>(settings->width) * 0.5f, static_cast<float>(settings->height) * 0.5f);

	gui.removeAllWidgets();
	gui.setWindow(game.window);

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();

	layoutWidth = static_cast<float>(settings->defWidgetWidth) * settings->widthScaleFactor;
	layoutHeight = static_cast<float>(settings->widgetHeight * 2.25) * settings->heightScaleFactor;
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	layout->setSize(layoutWidth, layoutHeight);
	layout->setPosition(layoutCenterX - layoutWidth * 0.5f, layoutCenterY - layoutHeight * 0.5f);

	gui.add(layout, "PauseMenu_Layout");

	tgui::Button::Ptr returnButton = std::make_shared<tgui::Button>();
	tgui::Button::Ptr quitButton = std::make_shared<tgui::Button>();

	returnButton->setText("Return to Game");
	quitButton->setText("Quit to Main Menu");

	returnButton->setTextSize(static_cast<unsigned int>(settings->buttonTextSize * settings->heightScaleFactor));
	quitButton->setTextSize(static_cast<unsigned int>(settings->buttonTextSize * settings->heightScaleFactor));

	returnButton->setOpacity(0.9f);
	quitButton->setOpacity(0.9f);
	returnButton->connect("pressed", [&](){ returnToGame(); }); // more beautiful lambda expression
	// returnButton->connect("pressed", &GameStatePauseMenu::returnToGame, *this); // ugly variant: 1st arg: function pointer, 2nd arg: instance -> value for this
	quitButton->connect("pressed", [&](){ quitToMainMenu(); }); // more beautiful lambda expression

	layout->add(returnButton);
	layout->add(quitButton);

	layout->insertSpace(1, 0.25);
}
