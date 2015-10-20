#include <SFML/Graphics.hpp>

#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "chances.hpp"

GameStateGame::GameStateGame(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(game.window.getSize());
	mapView.setSize(size);
	messageView.setSize(size);
	playerView.setSize(size);
	inventoryView.setSize(size);
	size *= 0.5f; // for positioning view centrally
	mapView.setCenter(size);
	messageView.setCenter(size);
	playerView.setCenter(size);
	inventoryView.setCenter(size);

	player.reset(new Player("player", 50.f, 25.f, 25.f, 25.f, settings->playerName, static_cast<float>(settings->tileSize), settings->maxInventorySize));
	player->setSize(settings->tileSize, settings->tileSize);

	map = new Map(game);
	map->init(player);
	
	player->init(map, settings->tileSize);
}

void GameStateGame::draw(const float deltaTime)
{
	game.window.setView(mapView);

	game.window.clear(sf::Color::Black);
	map->draw(game.window, deltaTime);
	player->draw(game.window, deltaTime);

	return;
}

void GameStateGame::update(const float deltaTime)
{
	player->update(deltaTime);
	map->update(deltaTime);
	return;
}

void GameStateGame::handleInput()
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
				game.window.close(); // turn to pause menu state
			}
			else
			{
				player->handleInput(event);
			}

			break;
		}
		default: break;
		}
	}

	// check all the window's events that were triggered since the last iteration of the loop
	//while (game.window.pollEvent(event))
	//{
	//	// "close requested" event: we close the window
	//	if (event.type == sf::Event::Closed) {
	//		game.window.close();
	//	}
	//	else if (event.type == sf::Event::KeyPressed) {
	//		settings->fullscreen = settings->fullscreen;
	//		game.window.create(sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH), settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

	//		settings->saveSettings();
	//	}

	//	return;
	//}
}

GameStateGame::~GameStateGame()
{
	delete map;
}
