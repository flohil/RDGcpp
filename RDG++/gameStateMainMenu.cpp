#include <SFML/Graphics.hpp>

#include "gameStateMainMenu.hpp"
#include "gameState.hpp"

GameStateMainMenu::GameStateMainMenu(Game& game_) : 
	GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(game.window.getSize());
	guiView.setSize(size);
	view.setSize(size);
	size *= 0.5f; // for positioning view centrally
	guiView.setCenter(size);
	view.setCenter(size);
}

void GameStateMainMenu::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(game.background);

	return;
}

void GameStateMainMenu::update(const float deltaTime)
{
	return;
}

void GameStateMainMenu::handleInput()
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
				if (event.key.code == sf::Keyboard::Escape) game.window.close();
				break;
			}
			default: break;
		}
	}

	// check all the window's events that were triggered since the last iteration of the loop
	while (game.window.pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed) {
			game.window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			settings->fullscreen = settings->fullscreen;
			game.window.create(sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH), settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

			settings->saveSettings();
		}

		return;
	}
}
