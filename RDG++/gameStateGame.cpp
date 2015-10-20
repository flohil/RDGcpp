#include <SFML/Graphics.hpp>

#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "chances.hpp"

GameStateGame::GameStateGame(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(static_cast<float>(settings->width), static_cast<float>(settings->height));
	//sf::Vector2f size = sf::Vector2f(1920, 1080);

	float aspectRatio = size.x / size.y;

	//1280 * 720 upwards! -> all 16:9

	// for aspect Ratio 16:9
	sf::Vector2f possibleMapViewSize = sf::Vector2f(size.x * 0.85f, size.y * 0.75f);
	viewportSize = possibleMapViewSize;
	sf::Vector2f messageViewSize = sf::Vector2f(size.x * 0.75f, size.y * 0.25f);
	sf::Vector2f playerViewSize = sf::Vector2f(size.x * 0.25f, size.y * 0.4f);
	sf::Vector2f inventoryViewSize = sf::Vector2f(size.x * 0.25f, size.y * 0.6f);

	unsigned int tilesX = (game_.getSettings()->ROOM_WIDTH + 1) * game_.getSettings()->mazeSize + 1;
	unsigned int tilesY = (game_.getSettings()->ROOM_HEIGHT + 1) * game_.getSettings()->mazeSize + 1;
	mapSize = sf::Vector2f(static_cast<float>(tilesX * settings->tileSize), static_cast<float>(tilesY * settings->tileSize));

	if (mapSize.x < possibleMapViewSize.x)
	{
		viewportSize.x = mapSize.x;
	}
	if (mapSize.y < possibleMapViewSize.y)
	{
		viewportSize.y = mapSize.y;
	}

	sf::Vector2f mapViewCenter = possibleMapViewSize * 0.5f;
	sf::Vector2f messageViewCenter = sf::Vector2f(messageViewSize.x * 0.5f, viewportSize.y + messageViewSize.y * 0.5f);
	sf::Vector2f playerViewCenter = sf::Vector2f(viewportSize.x + playerViewSize.x * 0.5f, playerViewSize.y * 0.5f);
	sf::Vector2f inventoryViewCenter = sf::Vector2f(viewportSize.x + inventoryViewSize.x * 0.5f, playerViewSize.y + inventoryViewSize.y * 0.5f);

	float left = (possibleMapViewSize.x - viewportSize.x) / size.x;
	float top = (possibleMapViewSize.y - viewportSize.y) / size.y;

	mapView.setViewport(sf::FloatRect(left / 2, top / 2, viewportSize.x / size.x, viewportSize.y / size.y));
	/*messageView.setViewport(sf::FloatRect(0.f, 0.f, horizontalSplit, verticalSplit));
	playerView.setViewport(sf::FloatRect(0.f, 0.f, horizontalSplit, verticalSplit));
	inventoryView.setViewport(sf::FloatRect(0.f, 0.f, horizontalSplit, verticalSplit));*/

	mapView.setSize(viewportSize);
	messageView.setSize(messageViewSize);
	playerView.setSize(playerViewSize);
	inventoryView.setSize(inventoryViewSize);
	mapView.setCenter(viewportSize * 0.5f);
	messageView.setCenter(messageViewCenter);
	playerView.setCenter(playerViewCenter);
	inventoryView.setCenter(inventoryViewCenter);

	borderMargin.x = static_cast<float>(settings->tileSize * (settings->ROOM_WIDTH + 1));
	borderMargin.y = static_cast<float>(settings->tileSize * (settings->ROOM_HEIGHT + 1));
	minTop = borderMargin.y;
	maxBottom = mapSize.y - borderMargin.y;
	minLeft = borderMargin.x;
	maxRight = mapSize.x - borderMargin.x;

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
	adaptMapViewport();

	return;
}

// adapt viewport of map to playerPosition
void GameStateGame::adaptMapViewport()
{
	sf::Vector2f& playerPos = player->getPixelPosition();

	//do not adapt viewport center if that would move the viewport beyond the map's borders
	const sf::Vector2f& center = mapView.getCenter();

	/*std::cout << "vp: left = " << vp.left << ", top = " << vp.top << ", width = " << vp.width << ", height = " << vp.height << std::endl;
	std::cout << "view size: " << mapView.getSize().x << " x " << mapView.getSize().y << std::endl;
	std::cout << "view center: " << mapView.getCenter().x << " x " << mapView.getCenter().y << std::endl;*/


	//float viewportTop = center.y + viewportSize.y * 0.5f;
	//float viewportBottom = center.y - viewportSize.y * 0.5f;
	//float viewportLeft = center.x - viewportSize.x * 0.5f;
	//float viewportRight = center.x + viewportSize.x * 0.5f;

	//// scroll to bottom
	//float scrollBottom = playerPos.y - (viewportBottom - borderMargin.y);
	//if (scrollBottom > 0 && viewportBottom < maxBottom)
	//{
	//	mapView.setCenter(center.x, center.y + scrollBottom);
	//}

	//// scroll to right
	//float scrollRight = playerPos.x - (viewportRight - borderMargin.x);
	//if (scrollRight > 0 && viewportRight < maxRight)
	//{
	//	mapView.setCenter(center.x + scrollRight, center.y);
	//}
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
