#include <SFML/Graphics.hpp>

#include "gameStateGame.hpp"
#include "gameStatePauseMenu.hpp"
#include "gameState.hpp"
#include "chances.hpp"
#include "easylogging++.hpp"
#include "OutputFormatter.hpp"

GameStateGame::GameStateGame(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	size = sf::Vector2f(static_cast<float>(settings->scaleWidth), static_cast<float>(settings->scaleHeight));

	float aspectRatio = size.x / size.y;

	//1280 * 720 upwards! -> all 16:9

	/*inventory margins:
		left & right: 10px
		top & bottom: 10px
		in between: 5px
	*/
	
	horSplit = 0.8265625f;
	rightVerSplit = 0.5f;
	verSplit = 0.75f;
	bottomHorSplit = 0.5f; // relative to horSplit

	float armorImageHeight = size.x * (1.f - horSplit);
	
	// for aspect Ratio 16:9
	sf::Vector2f possibleMapViewSize = sf::Vector2f(size.x * horSplit, size.y * 0.75f);
	viewportSize = possibleMapViewSize;
	sf::Vector2f chatViewSize = sf::Vector2f(size.x * horSplit * bottomHorSplit, size.y * (1 - verSplit));
	sf::Vector2f detailsViewSize = sf::Vector2f(size.x * horSplit * bottomHorSplit, size.y * (1 - verSplit));
	sf::Vector2f armorViewSize = sf::Vector2f(size.x * (1.f - horSplit), size.y * rightVerSplit);
	sf::Vector2f inventoryViewSize = sf::Vector2f(size.x * (1.f - horSplit), size.y * (1.f - rightVerSplit));

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
	armorView.setViewport(sf::FloatRect(horSplit, 0.f, (1.f - horSplit), rightVerSplit));
	chatView.setViewport(sf::FloatRect(0.f, verSplit, horSplit * bottomHorSplit, (1.f - verSplit)));
	detailsView.setViewport(sf::FloatRect(horSplit * bottomHorSplit, verSplit, (horSplit - horSplit * bottomHorSplit), (1.f - verSplit)));
	inventoryView.setViewport(sf::FloatRect(horSplit, rightVerSplit, (1.f - horSplit), (1.f - rightVerSplit)));

	mapView.setSize(viewportSize);
	armorView.setSize(armorViewSize);
	chatView.setSize(chatViewSize);
	detailsView.setSize(detailsViewSize);
	inventoryView.setSize(inventoryViewSize);
	mapView.setCenter(viewportSize * 0.5f);
	armorView.setCenter(armorViewSize * 0.5f);
	chatView.setCenter(chatViewSize * 0.5f);
	detailsView.setCenter(detailsViewSize * 0.5f);
	inventoryView.setCenter(inventoryViewSize * 0.5f);

	// for camera center adaption
	borderMargin.x = static_cast<float>(settings->tileSize * (settings->ROOM_WIDTH + 1));
	borderMargin.y = static_cast<float>(settings->tileSize * (settings->ROOM_HEIGHT + 1));
	minTop = borderMargin.y;
	maxBottom = mapSize.y - borderMargin.y;
	minLeft = borderMargin.x;
	maxRight = mapSize.x - borderMargin.x;

	armorSprite.setTexture(ResourceManager::getInstance().getTexture("armorBackground"));
	armorSprite.setPosition(0.f, 50.f);
	armorSprite.setScale(sf::Vector2f(0.65f, 0.65f));

	std::cout << "armorView x = " << armorView.getSize().x << ", y = " << armorView.getSize().y << std::endl;
	std::cout << "armorView center x = " << armorView.getCenter().x << ", y = " << armorView.getCenter().y << std::endl;
	std::cout << "armorView viewport left = " << armorView.getViewport().left << ", top = " << armorView.getViewport().top << ", width = " << armorView.getViewport().width << ", height = " << armorView.getViewport().height << std::endl;

	player.reset(new Player("player", 50.f, 25.f, 25.f, 25.f, settings->playerName, static_cast<float>(settings->tileSize), settings->maxInventorySize));
	player->setSize(settings->tileSize, settings->tileSize);

	map = new Map(game);
	map->init(player);

	theme = std::make_shared<tgui::Theme>(settings->IMAGE_PATH + "widgets/Black.txt");

	loadGui();

	player->init(map, settings->tileSize, chatbox);

	OutputFormatter::chat(chatbox, "Hello " + player->getPlayerName() + ", welcome to the Dungeon!", sf::Color::White);
}

void GameStateGame::draw(const float deltaTime)
{
	game.window.clear(sf::Color::Black);

	game.window.setView(mapView);
	fightGui.draw();
	map->draw(game.window, deltaTime);
	player->draw(game.window, deltaTime);
	
	game.window.setView(armorView);
	armorGui.draw();
	game.window.draw(armorSprite);

	game.window.setView(inventoryView);
	inventoryGui.draw();

	game.window.setView(chatView);
	chatGui.draw();

	game.window.setView(detailsView);
	detailsGui.draw();

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

		chatGui.handleEvent(event);
		// detailsGui.handleEvent(event);
		fightGui.handleEvent(event);
		armorGui.handleEvent(event);
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

	chatGui.removeAllWidgets();
	chatGui.setWindow(game.window);

	detailsGui.removeAllWidgets();
	detailsGui.setWindow(game.window);

	fightGui.removeAllWidgets();
	fightGui.setWindow(game.window);

	armorGui.removeAllWidgets();
	armorGui.setWindow(game.window);

	inventoryGui.removeAllWidgets();
	inventoryGui.setWindow(game.window);

	// set global font that all widgets can use by default
	chatGui.setFont("res/fonts/DejaVuSans.ttf");
	detailsGui.setFont("res/fonts/DejaVuSans.ttf");
	armorGui.setFont("res/fonts/DejaVuSans.ttf");
	inventoryGui.setFont("res/fonts/DejaVuSans.ttf");
	fightGui.setFont("res/fonts/DejaVuSans.ttf");

	// gui loading
	chatbox = theme->load("ChatBox");
	chatbox->setSize(size.x * horSplit * bottomHorSplit, size.y * (1 - verSplit));
	chatbox->setTextSize(18);
	chatbox->setPosition(0, 0);
	chatbox->setLineLimit(8);
	chatbox->setLinesStartFromTop();
	chatGui.add(chatbox, "log");

	chatGui.setView(chatView);

	detailsbox = theme->load("ChatBox");
	detailsbox->setSize(size.x * horSplit * bottomHorSplit, size.y * (1 - verSplit));
	detailsGui.add(detailsbox, "details");

	detailsGui.setView(detailsView);

	inventorybox = theme->load("ChatBox");
	inventorybox->setSize(size.x * (1 - horSplit), size.y * (1.f - rightVerSplit));
	inventorybox->setPosition(0, 0);
	inventoryGui.add(inventorybox, "inventory");

	inventoryGui.setView(inventoryView);

	armorbox = theme->load("ChatBox");
	armorbox->setSize(size.x * (1 - horSplit), size.y * rightVerSplit);
	armorbox->setPosition(0, 0);
	armorGui.add(armorbox, "armor");

	tgui::Button::Ptr set1Button = theme->load("Button");
	set1Button->setText("Set 1");
	set1Button->setOpacity(1.f);
	set1Button->setPosition(4, 4);
	set1Button->setSize(size.x * (1 - horSplit) * 0.5f - 4, 35);
	set1Button->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * 18));
	//set1Button->connect("pressed", [&](){ startGame(); });
	armorGui.add(set1Button);

	tgui::Button::Ptr set2Button = theme->load("Button");
	set2Button->setText("Set 2");
	set2Button->setOpacity(1.f);
	set2Button->setPosition(set1Button->getSize().x + 6, 4);
	set2Button->setSize(size.x * (1 - horSplit) * 0.5f - 4, 35);
	set2Button->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * 18));
	//set2Button->connect("pressed", [&](){ startGame(); });
	armorGui.add(set2Button);

	armorGui.setView(armorView);

}

GameStateGame::~GameStateGame()
{
	delete map;
}