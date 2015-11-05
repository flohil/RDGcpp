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
	
	//horSplit = 0.8265625f;
	horSplit = 0.815f;
	rightVerSplit = 0.5f;
	verSplit = 0.75f;
	bottomHorSplit = 0.5f; // relative to horSplit

	horSplitAbs = static_cast<int>(horSplit * size.x);
	rightVerSplitAbs = static_cast<int>(rightVerSplit * size.y);
	verSplitAbs = static_cast<int>(verSplit * size.y);
	bottomHorSplitAbs = static_cast<int>(bottomHorSplit * size.y);

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
	completeView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	mapView.setSize(viewportSize);
	armorView.setSize(armorViewSize);
	chatView.setSize(chatViewSize);
	detailsView.setSize(detailsViewSize);
	inventoryView.setSize(inventoryViewSize);
	completeView.setSize(size);
	mapView.setCenter(viewportSize * 0.5f);
	armorView.setCenter(armorViewSize * 0.5f);
	chatView.setCenter(chatViewSize * 0.5f);
	detailsView.setCenter(detailsViewSize * 0.5f);
	inventoryView.setCenter(inventoryViewSize * 0.5f);
	completeView.setCenter(size * 0.5f);

	// for camera center adaption
	borderMargin.x = static_cast<float>(settings->tileSize * (settings->ROOM_WIDTH + 1));
	borderMargin.y = static_cast<float>(settings->tileSize * (settings->ROOM_HEIGHT + 1));
	minTop = borderMargin.y;
	maxBottom = mapSize.y - borderMargin.y;
	minLeft = borderMargin.x;
	maxRight = mapSize.x - borderMargin.x;

	armorSprite.setTexture(ResourceManager::getInstance().getTexture("armorBackground"));
	potionSprite.setTexture(ResourceManager::getInstance().getTexture("potionBar"));

	player.reset(new Player("player", 50.f, 25.f, 25.f, 25.f, settings->playerName, static_cast<float>(settings->tileSize), settings->maxInventorySize, sf::Vector2f(armorLeftOffset, armorTopOffset), sf::Vector2f(potionLeftOffset, potionTopOffset)));
	player->setSize(settings->tileSize, settings->tileSize);

	map = new Map(game);
	map->init(player);

	theme = std::make_shared<tgui::Theme>(settings->IMAGE_PATH + "widgets/Black.txt");

	loadGui();

	player->init(map, settings->tileSize, chatbox, horSplitAbs, rightVerSplitAbs);

	OutputFormatter::chat(chatbox, "Hello " + player->getPlayerName() + ", welcome to the Dungeon!", sf::Color::White);

	// test fill set
	player->getEquipmentSet()->setBoots(game.getPrototypeStorage()->armamentFactory->create("Leather Boots"));
	player->getEquipmentSet()->setCuisse(game.getPrototypeStorage()->armamentFactory->create("Leather Cuisse"));
	player->getEquipmentSet()->setGauntlets(game.getPrototypeStorage()->armamentFactory->create("Leather Gauntlets"));
	player->getEquipmentSet()->setHarness(game.getPrototypeStorage()->armamentFactory->create("Leather Harness"));
	player->getEquipmentSet()->setHelmet(game.getPrototypeStorage()->armamentFactory->create("Leather Helmet"));
	player->getEquipmentSet()->setPrimaryWeapon(game.getPrototypeStorage()->weaponFactory->create("Dagger"));
	player->getEquipmentSet()->setSecondaryWeapon(game.getPrototypeStorage()->weaponFactory->create("Axt"));
	player->getEquipmentSet()->setPotion1(game.getPrototypeStorage()->potionFactory->create("Antidote"));
	player->getEquipmentSet()->setPotion2(game.getPrototypeStorage()->potionFactory->create("Antidote"));
	player->getEquipmentSet()->setPotion3(game.getPrototypeStorage()->potionFactory->create("Antidote"));
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
	game.window.draw(potionSprite);
	player->drawEquipment(game.window, deltaTime);

	game.window.setView(inventoryView);
	inventoryGui.draw();
	player->drawInventory(game.window, deltaTime);

	game.window.setView(chatView);
	chatGui.draw();

	game.window.setView(detailsView);
	detailsGui.draw();

	game.window.setView(completeView);
	if (draggedItem != nullptr && dragging)
	{
		draggedItem->draw(game.window, deltaTime);
	}

	return;
}

void GameStateGame::update(const float deltaTime)
{
	player->update(deltaTime);
	map->update(deltaTime);
	adaptMapViewport();

	if (mouseDeterminationTriggered)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);

		if (mousePos.x != lastClickPos.x || mousePos.y != lastClickPos.y)
		{
			dragging = true;
			handleMouseEvent(lastClickPos, MouseEvent::DRAGSTART);
			mouseDeterminationTriggered = false;
		}
	}

	if (dragging)
	{
		if (draggedItem != nullptr)
		{
			draggedItem->setCenteredPosition(sf::Mouse::getPosition(game.window));
		}
	}

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
				player->handleInput(event, draggedItem);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				mouseDeterminationTriggered = true;
				lastClickPos = sf::Mouse::getPosition(game.window);
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (!dragging)
				{
					handleMouseEvent(sf::Mouse::getPosition(game.window), MouseEvent::CLICK);
				}
				else
				{
					handleMouseEvent(sf::Mouse::getPosition(game.window), MouseEvent::DRAGRELEASE);
				}

				mouseDeterminationTriggered = false;
				dragging = false;
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

	float armorButtonsTopMargin = 10.f;
	float armorButtonsLeftMargin = 8.f;
	float spacing = 2.f;
	sf::Vector2f setButtonSize(size.x * (1 - horSplit) * 0.5f - armorButtonsLeftMargin - spacing, 35);
	unsigned int setButtonTextSize = 18u;

	set1Button = theme->load("Button");
	set1Button->setText("Set 1");
	set1Button->setPosition(armorButtonsLeftMargin, armorButtonsTopMargin);
	set1Button->setSize(setButtonSize.x, setButtonSize.y);
	set1Button->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * setButtonTextSize));
	set1Button->connect("pressed", [&](){ changeSet(1); });
	armorGui.add(set1Button);

	set2Button = theme->load("Button");
	set2Button->setText("Set 2");
	set2Button->setPosition(armorButtonsLeftMargin + setButtonSize.x + 2 * spacing, armorButtonsTopMargin);
	set2Button->setSize(setButtonSize.x, setButtonSize.y);
	set2Button->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * setButtonTextSize));
	set2Button->connect("pressed", [&](){ changeSet(2); });
	armorGui.add(set2Button);

	changeSet(player->getEquipmentSet()->getNumerator());

	armorTopOffset = armorButtonsTopMargin + setButtonSize.y + 10.f;
	armorLeftOffset = -1.f;
	potionTopOffset = rightVerSplit * size.y - 47.f;
	potionLeftOffset = -2.f;

	armorSprite.setPosition(armorLeftOffset, armorTopOffset);
	armorSprite.setScale(sf::Vector2f(0.7f, 0.7f));
	potionSprite.setPosition(potionLeftOffset, rightVerSplit * size.y - 47.f);
	potionSprite.setScale(sf::Vector2f(1.1f, 1.1f));

	sf::Vector2f armorDims;
	sf::Vector2f potionDims;

	armorDims.x = armorSprite.getGlobalBounds().width;
	armorDims.y = armorSprite.getGlobalBounds().height;
	potionDims.x = potionSprite.getGlobalBounds().width;
	potionDims.y = potionSprite.getGlobalBounds().height;

	player->setEquipmentOffsets(sf::Vector2f(armorLeftOffset, armorTopOffset), sf::Vector2f(potionLeftOffset, potionTopOffset), armorDims, potionDims, horSplitAbs, rightVerSplitAbs);

	armorGui.setView(armorView);

	float detailsLeftMargin = 30.f;
	float detailsTopMargin = 10.f;
	float detailsWidth = detailsView.getSize().x - detailsLeftMargin * 2;
	float detailsHeight = detailsView.getSize().y - detailsLeftMargin * 2;

	tgui::VerticalLayout::Ptr detailsLayout = std::make_shared<tgui::VerticalLayout>();
	tgui::HorizontalLayout::Ptr keysLayout = std::make_shared<tgui::HorizontalLayout>();
	tgui::HorizontalLayout::Ptr valuesLayout = std::make_shared<tgui::HorizontalLayout>();

	detailsLayout->setSize(detailsWidth, detailsHeight);
	detailsLayout->setPosition(detailsLeftMargin, detailsTopMargin);

	/*keysLayout->setSize(detailsLayout->getSize().x * 0.5, detailsLayout->getSize().y);
	valuesLayout->setSize(detailsLayout->getSize().x * 0.5, detailsLayout->getSize().y);*/

	tgui::Label::Ptr detailsTitleLabel = theme->load("Label");
}

void GameStateGame::changeSet(unsigned int numerator)
{
	float activeOpacity = 1.f;
	float inactiveOpacity = 0.7f;

	player->setActiveEquipmentSet(numerator);
	if (numerator == 1u)
	{
		set1Button->setOpacity(activeOpacity);
		set2Button->setOpacity(inactiveOpacity);
	}
	else
	{
		set1Button->setOpacity(inactiveOpacity);
		set2Button->setOpacity(activeOpacity);
	}
}

void GameStateGame::handleMouseEvent(sf::Vector2i pos_, MouseEvent::Enum eventType)
{
	// scale mouse position
	sf::Vector2i pos;

	pos.x = static_cast<int>(pos_.x * static_cast<float>(size.x / settings->width));
	pos.y = static_cast<int>(pos_.y * static_cast<float>(size.y / settings->height));

	std::cout << "mouseEvent: pos = " << pos.x << ", " << pos.y << ", type: " << eventType << std::endl;

	if (eventType == MouseEvent::DRAGSTART)
	{
		dragStartPos = pos; // in case anything goes wrong when dropping an item, return it to former position
	}

	if (pos.x < horSplitAbs && pos.y < verSplitAbs) // inside map
	{
		if (eventType == MouseEvent::CLICK)
		{
			std::cout << map->getItemAtPixels(pos) << std::endl;
		}
		else if (eventType == MouseEvent::DRAGRELEASE)
		{
			if (draggedItem != nullptr)
			{
				Point facingPoint = player->getPlayerPosition().getDirPoint(player->getFacingDir());
				if (map->getOverlayObject(facingPoint) == nullptr)
				{
					OutputFormatter::chat(chatbox, "Dropped " + draggedItem->getName(), sf::Color::White);
					map->setOverlayObject(facingPoint, draggedItem); // place dragged item on map if there is a free tile in front of the player
					draggedItem = nullptr;
				}
				else
				{
					handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE); // return dragged item to where it came from if there is no free tile in front of the player
				}
			}
			dragging = false;
		}
	}
	else if (pos.x > horSplitAbs && pos.y < rightVerSplitAbs) // inside armor
	{
		if (eventType == MouseEvent::DRAGSTART)
		{
			draggedItem = player->getArmorItemAtPixels(pos, true);

			std::cout << "draggedItem: " << draggedItem << std::endl;

			if (draggedItem != nullptr)
			{
				draggedItem->setSize(settings->tileSize, settings->tileSize);
			}
		}
		else if (eventType == MouseEvent::CLICK)
		{
			std::cout << player->getArmorItemAtPixels(pos) << std::endl;
		}
		else if (eventType == MouseEvent::DRAGRELEASE)
		{
			player->getEquipmentSet()->setItemAtPixels(pos, draggedItem);
		}
	}
	else if (pos.x > horSplitAbs && pos.y >= rightVerSplitAbs) // inside inventory
	{
		if (eventType == MouseEvent::DRAGSTART)
		{
			draggedItem = player->getInventoryItemAtPixels(pos, true);

			if (draggedItem != nullptr)
			{
				draggedItem->setSize(settings->tileSize, settings->tileSize);
			}
		}
		else if(eventType == MouseEvent::CLICK)
		{
			std::cout << player->getInventoryItemAtPixels(pos) << std::endl;
		}
		else if (eventType == MouseEvent::DRAGRELEASE)
		{
			if ((draggedItem = player->putInInventory(draggedItem, false)) != nullptr)
			{
				handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE);
			}
			dragging = false;
		}
	}
	else
	{
		if (eventType == MouseEvent::DRAGRELEASE)
		{
			handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE);
			dragging = false;
		}
	}
}

GameStateGame::~GameStateGame()
{
	delete map;
}