#include <SFML/Graphics.hpp>

#include "gameStatePauseMenu.hpp"
#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

GameStatePauseMenu::GameStatePauseMenu(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(game.window.getSize());
	guiView.setSize(size);
	view.setSize(size);
	size *= 0.5f; // for positioning view centrally
	guiView.setCenter(size);
	view.setCenter(size);

	background.setTexture(ResourceManager::getInstance().getTexture("background"));
	background.setTextureRect(sf::IntRect(0, 0, settings->width, settings->height));

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
		switch (event.type)
		{
			/* Close the window */
			case sf::Event::Closed:
			{
				game.window.close();
				break;
			}
			/* Resize the window */
			case sf::Event::KeyPressed:
			{
				// just sample code - remove later on
				if (event.key.code == sf::Keyboard::Escape)
				{
					returnToGame();
				}
				break;
			}
			default:
			{
				gui.handleEvent(event);
				break;
			}
		}
	}
}

void GameStatePauseMenu::returnToGame()
{
	LOG(INFO) << "Returning to game";

	game.popState();

	return;
}

void GameStatePauseMenu::quitToMainMenu()
{
	LOG(INFO) << "Quitting to main menu";

	game.popState();
	game.popState();

	return;
}

void GameStatePauseMenu::loadGui()
{
	gui.setWindow(game.window);

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();

	layoutWidth = 300;
	layoutHeight = 130;
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	layout->setSize(layoutWidth, layoutHeight);
	layout->setPosition(layoutCenterX - layoutWidth * 0.5f, layoutCenterY - layoutHeight * 0.5f);

	gui.add(layout, "PauseMenu_Layout");

	tgui::Button::Ptr returnButton = std::make_shared<tgui::Button>();
	tgui::Button::Ptr quitButton = std::make_shared<tgui::Button>();

	layout->add(returnButton, "ReturnToGame_Button");
	layout->add(quitButton, "QuitToMainMenu_Button");

	layout->insertSpace(1, 0.5);

	returnButton->setText("Return to Game");
	quitButton->setText("Quit to Main Menu");

	std::cout << "text: " << returnButton->getText().toAnsiString() << std::endl;

	returnButton->setOpacity(0.9f);
	quitButton->setOpacity(0.9f);
	returnButton->connect("pressed", [&](){ returnToGame(); }); // more beautiful lambda expression
	// returnButton->connect("pressed", &GameStatePauseMenu::returnToGame, *this); // ugly variant: 1st arg: function pointer, 2nd arg: instance -> value for this
	quitButton->connect("pressed", [&](){ quitToMainMenu(); }); // more beautiful lambda expression
}
