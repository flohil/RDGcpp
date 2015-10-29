#include <SFML/Graphics.hpp>

#include "gameStateGame.hpp"
#include "gameStatePauseMenu.hpp"
#include "gameState.hpp"
#include "chances.hpp"
#include "easylogging++.hpp"

GameStateGame::GameStateGame(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(1280, 720);

	float aspectRatio = size.x / size.y;

	//1280 * 720 upwards! -> all 16:9

	float horSplit = 0.85f;
	float rightVerSplit = 0.4f;
	float verSplit = 0.75f;
	float bottomHorSplit = 0.4f;
	
	// for aspect Ratio 16:9
	sf::Vector2f possibleMapViewSize = sf::Vector2f(size.x * horSplit, size.y * 0.75f);
	viewportSize = possibleMapViewSize;
	sf::Vector2f messageViewSize = sf::Vector2f(size.x * 0.5f, size.y * 0.25f);
	sf::Vector2f detailViewSize = sf::Vector2f(size.x * 0.5f, size.y * 0.25f);
	sf::Vector2f armorViewSize = sf::Vector2f(size.x * (1.f - horSplit), size.y * rightVerSplit);
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

	float left = (possibleMapViewSize.x - viewportSize.x) / size.x;
	float top = (possibleMapViewSize.y - viewportSize.y) / size.y;

	mapView.setViewport(sf::FloatRect(left / 2, top / 2, viewportSize.x / size.x, viewportSize.y / size.y));
	armorView.setViewport(sf::FloatRect(0.85f, 0.f, 0.15f, size.x / size.y * 0.15f));
	

	armorViewSize = sf::Vector2f(size.x * 0.15f, size.x * 0.15f);

	mapView.setSize(viewportSize);
	armorView.setSize(armorViewSize);
	mapView.setCenter(viewportSize * 0.5f);
	armorView.setCenter(armorViewSize * 0.5f);

	borderMargin.x = static_cast<float>(settings->tileSize * (settings->ROOM_WIDTH + 1));
	borderMargin.y = static_cast<float>(settings->tileSize * (settings->ROOM_HEIGHT + 1));
	minTop = borderMargin.y;
	maxBottom = mapSize.y - borderMargin.y;
	minLeft = borderMargin.x;
	maxRight = mapSize.x - borderMargin.x;

	armorSprite.setTexture(ResourceManager::getInstance().getTexture("armorBackground"));
	armorSprite.setPosition(0.f, 0.f);
	armorSprite.setScale(sf::Vector2f(1.2f, 1.2f));

	std::cout << "armorView x = " << armorView.getSize().x << ", y = " << armorView.getSize().y << std::endl;
	std::cout << "armorView center x = " << armorView.getCenter().x << ", y = " << armorView.getCenter().y << std::endl;
	std::cout << "armorView viewport left = " << armorView.getViewport().left << ", top = " << armorView.getViewport().top << ", width = " << armorView.getViewport().width << ", height = " << armorView.getViewport().height << std::endl;

	player.reset(new Player("player", 50.f, 25.f, 25.f, 25.f, settings->playerName, static_cast<float>(settings->tileSize), settings->maxInventorySize));
	player->setSize(settings->tileSize, settings->tileSize);

	map = new Map(game);
	map->init(player);
	
	player->init(map, settings->tileSize);
}

void GameStateGame::draw(const float deltaTime)
{
	game.window.clear(sf::Color::Black);

	game.window.setView(mapView);
	map->draw(game.window, deltaTime);
	player->draw(game.window, deltaTime);
	
	game.window.setView(armorView);
	game.window.draw(armorSprite);
	

	/*game.window.setView(inventoryView);

	game.window.setView(messageView);

	game.window.setView(detailView);*/

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

	float viewportTop = center.y - viewportSize.y * 0.5f;
	float viewportBottom = center.y + viewportSize.y * 0.5f;
	float viewportLeft = center.x - viewportSize.x * 0.5f;
	float viewportRight = center.x + viewportSize.x * 0.5f;

	// scroll to bottom
	float scrollBottom = playerPos.y - (viewportBottom - borderMargin.y);
	if (scrollBottom > 0)
	{
		if (playerPos.y < maxBottom)
		{
			mapView.setCenter(center.x, center.y + scrollBottom);
		}
	}

	// scroll to right
	float scrollRight = playerPos.x - (viewportRight - borderMargin.x);
	if (scrollRight > 0)
	{
		if (playerPos.x < maxRight)
		{
			mapView.setCenter(center.x + scrollRight, center.y);
		}
	}

	// scroll to top
	float scrollTop = (viewportTop + borderMargin.y) - playerPos.y;
	if (scrollTop > 0)
	{
		if (playerPos.y > minTop)
		{
			mapView.setCenter(center.x, center.y - scrollTop);
		}
	}

	// scroll to left
	float scrollLeft = (viewportLeft + borderMargin.x) - playerPos.x;
	if (scrollLeft > 0)
	{
		if (playerPos.x > minLeft)
		{
			mapView.setCenter(center.x - scrollLeft, center.y);
		}
	}
}

void GameStateGame::handleInput()
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
			if (event.key.code == sf::Keyboard::Delete)
			{
				game.window.close();
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				pauseGame();
			}
			else
			{
				player->handleInput(event);
			}
		}

		// gui.handleEvent(event);
	}
}

void GameStateGame::pauseGame()
{
	LOG(INFO) << "Pausing game";

	game.pushState(std::shared_ptr<GameState>(new GameStatePauseMenu(game)));

	return;
}

void GameStateGame::loadGui()
{
	// gui loading

}

GameStateGame::~GameStateGame()
{
	delete map;
}
