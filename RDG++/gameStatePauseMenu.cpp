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

	game.backgroundSprite.setTexture(ResourceManager::getInstance().getTexture("Frog"));
	game.backgroundSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	game.backgroundSprite.setScale(sf::Vector2f(0.5f, 0.5f));
}

void GameStatePauseMenu::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(game.backgroundSprite);

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
		default: break;
		}
	}
}

void GameStatePauseMenu::returnToGame()
{
	LOG(INFO) << "Returning to game";

	game.popState();

	return;
}
