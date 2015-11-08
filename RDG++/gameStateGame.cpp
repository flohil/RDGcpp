#include <SFML/Graphics.hpp>

#include "gameStateGame.hpp"
#include "gameStatePauseMenu.hpp"
#include "gameState.hpp"
#include "chances.hpp"
#include "easylogging++.hpp"
#include "OutputFormatter.hpp"

void DetailsBag::addRow(std::string key, std::string value)
{
	std::vector<std::string> row;
	row.push_back(key);
	row.push_back(value);
	details.push_back(row);
}

DetailsBag::DetailsBag(std::shared_ptr<RenderableObject> obj) : detailsPic(ResourceManager::getInstance().getTexture(obj->getName()))
{
	std::cout << "constructing detailsBag for GameObject " << obj->getName() << std::endl;

	if (obj->getObjectType() == ObjectType::KEY)
	{
		name = "Key";
		addRow("description", "unlocks treasure chamber door");
	}
	else if (obj->getObjectType() == ObjectType::ITEM)
	{
		std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(obj);

		name = item->getName();

		if (item->getItemType() == ItemType::ARMAMENT)
		{
			std::shared_ptr<Armament> armament = std::dynamic_pointer_cast<Armament>(item);

			addRow("item class", EnumMapper::mapClassesName(armament->getItemClass()));
			addRow("armament type", EnumMapper::mapArmamentTypeNames(armament->getArmamentType()));
			addRow("material", armament->getMaterial());
			addRow("armor", OutputFormatter::shortFloat(armament->getArmor()));
			addRow("speed", OutputFormatter::shortFloat(armament->getSpeed()));
			addRow("full set multiplier", OutputFormatter::shortFloat(armament->getBonus() + 1.f));
		}
		else if (item->getItemType() == ItemType::WEAPON)
		{
			std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(item);

			addRow("item class", EnumMapper::mapClassesName(weapon->getItemClass()));
			addRow("weapon type", EnumMapper::mapWeaponTypeName(weapon->getType()));
			addRow("attack", OutputFormatter::shortFloat(weapon->getAttack()));
			addRow("speed", OutputFormatter::shortFloat(weapon->getSpeed()));
			addRow("accuracy", OutputFormatter::shortFloat(weapon->getAccuracy()));
			addRow("defence", OutputFormatter::shortFloat(weapon->getDefence()));
		}
		else if (item->getItemType() == ItemType::POTION)
		{
			std::shared_ptr<Potion> potion = std::dynamic_pointer_cast<Potion>(item);

			std::string description = potion->getDescription();
			size_t findN = description.find(" n ");
			size_t findX = description.find(" x ");

			// replace n
			if (findN != std::string::npos)
			{
				description.replace(findN + 1, 1, std::to_string(potion->getDuration()));
			}
			if (findX != std::string::npos)
			{
				description.replace(findX + 1, 1, OutputFormatter::shortFloat(potion->getStrength()));
			}

			std::cout << "description: " << description << std::endl;

			addRow("description", description);
			addRow("item class", EnumMapper::mapClassesName(potion->getItemClass()));
		}
	}
	else if (obj->getObjectType() == ObjectType::CREATURE)
	{
		std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(obj);

		if (creature->getCreatureType() == CreatureType::MONSTER)
		{
			std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(creature);

			name = monster->getName();

			addRow("level", EnumMapper::mapLevelName(monster->getLevel()));
			addRow("hp", OutputFormatter::shortFloat(monster->hp) + " (" + OutputFormatter::shortFloat(monster->getOrHP()) + ")");
			addRow("strength", OutputFormatter::shortFloat(monster->strength) + " (" + OutputFormatter::shortFloat(monster->getOrStrength()) + ")");
			addRow("speed", OutputFormatter::shortFloat(monster->speed) + " (" + OutputFormatter::shortFloat(monster->getOrSpeed()) + ")");
			addRow("accuracy", OutputFormatter::shortFloat(monster->accuracy) + " (" + OutputFormatter::shortFloat(monster->getOrAccuracy()) + ")");
			addRow("kill bonus", OutputFormatter::shortFloat(monster->getKillBonus()) + "  " + EnumMapper::mapAttributeName(monster->getKillBonusType()));
		}
	}
	else
	{
		name = "";
	}
}

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
	if (inFight)
	{
		fightGui.draw();
	}
	else
	{
		map->draw(game.window, deltaTime);
		player->draw(game.window, deltaTime);
	}
	
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
	if (detailsHeader->getText() != "")
	{
		game.window.draw(detailsSprite);
	}

	game.window.setView(completeView);
	if (draggedItem != nullptr && dragging)
	{
		draggedItem->draw(game.window, deltaTime);
	}

	return;
}

void GameStateGame::update(const float deltaTime)
{
	if (!inFight)
	{
		player->update(deltaTime); // move player

		std::shared_ptr<Monster> pendingFightEnemy = player->getPendingFightEnemy();

		if (pendingFightEnemy != nullptr)
		{
			startFight(player, pendingFightEnemy);
		}
	}
	
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
				if (inFight)
				{
					fight.reset();
					player->setPendingFightEnemy(nullptr);
					inFight = false;
				}
				else
				{
					game.window.close();
				}
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

	player->setChatbox(chatbox);

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

	/* FIGHT STUFF */
	float spacer = 30.f;
	float buttonWidth = (horSplitAbs - spacer) / 4.f;
	float buttonHeight = verSplitAbs / 8.f;
	float buttonCount = 4.f;
	float spacePerButton = spacer / buttonCount;
	float buttonWithSpacer = buttonWidth + spacePerButton;
	float heightSpacer = 6.f;

	fightGui.setView(mapView);

	attackButton = theme->load("Button");
	attackButton->setText("Attack");
	attackButton->setOpacity(1.f);
	attackButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	attackButton->connect("pressed", [&](){ toggleAttackGui(); });
	attackButton->setSize(buttonWidth, buttonHeight);
	attackButton->setPosition(0.f * buttonWithSpacer + spacePerButton /2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer);
	fightGui.add(attackButton);

	parryButton = theme->load("Button");
	parryButton->setText("Parry");
	parryButton->setOpacity(1.f);
	parryButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	parryButton->connect("pressed", [&](){ parry(); });
	parryButton->setSize(buttonWidth, buttonHeight);
	parryButton->setPosition(1.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer);
	fightGui.add(parryButton);

	potionButton = theme->load("Button");
	potionButton->setText("Use Potion");
	potionButton->setOpacity(1.f);
	potionButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	potionButton->connect("pressed", [&](){ usePotion(); });
	potionButton->setSize(buttonWidth, buttonHeight);
	potionButton->setPosition(2.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer);
	fightGui.add(potionButton);

	equipmentSetButton = theme->load("Button");
	equipmentSetButton->setText("Change Equipment");
	equipmentSetButton->setOpacity(1.f);
	equipmentSetButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	equipmentSetButton->connect("pressed", [&](){ toggleEquipment(); });
	equipmentSetButton->setSize(buttonWidth, buttonHeight);
	equipmentSetButton->setPosition(3.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer);
	fightGui.add(equipmentSetButton);

	/* Attack Submenu */
	headButton = theme->load("Button");
	headButton->setText("Head");
	headButton->setOpacity(1.f);
	headButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	headButton->connect("pressed", [&](){ attackHead(); });
	headButton->setSize(buttonWidth, buttonHeight);
	headButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer - 1.f * buttonHeight);

	torsoButton = theme->load("Button");
	torsoButton->setText("Torso");
	torsoButton->setOpacity(1.f);
	torsoButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	torsoButton->connect("pressed", [&](){ attackTorso(); });
	torsoButton->setSize(buttonWidth, buttonHeight);
	torsoButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer - 2.f * buttonHeight);

	armsButton = theme->load("Button");
	armsButton->setText("Arms");
	armsButton->setOpacity(1.f);
	armsButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	armsButton->connect("pressed", [&](){ attackArms(); });
	armsButton->setSize(buttonWidth, buttonHeight);
	armsButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer - 3.f * buttonHeight);

	legsButton = theme->load("Button");
	legsButton->setText("Legs");
	legsButton->setOpacity(1.f);
	legsButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	legsButton->connect("pressed", [&](){ attackLegs(); });
	legsButton->setSize(buttonWidth, buttonHeight);
	legsButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, verSplitAbs - verSplitAbs / 8.f - heightSpacer - 4.f * buttonHeight);

	/* FIGHT STUFF END */

	float armorButtonsTopMargin = 10.f;
	float armorButtonsLeftMargin = 8.f;
	float spacing = 2.f;
	sf::Vector2f setButtonSize(size.x * (1 - horSplit) * 0.5f - armorButtonsLeftMargin - spacing, 35);
	unsigned int setButtonTextSize = 18u;

	set1Button = theme->load("Button");
	set1Button->setText("Set 1");
	set1Button->setPosition(armorButtonsLeftMargin, armorButtonsTopMargin);
	set1Button->setSize(setButtonSize.x, setButtonSize.y);
	set1Button->setTextSize(static_cast<unsigned int>(setButtonTextSize));
	set1Button->connect("pressed", [&](){ changeSet(1); });
	armorGui.add(set1Button);

	set2Button = theme->load("Button");
	set2Button->setText("Set 2");
	set2Button->setPosition(armorButtonsLeftMargin + setButtonSize.x + 2 * spacing, armorButtonsTopMargin);
	set2Button->setSize(setButtonSize.x, setButtonSize.y);
	set2Button->setTextSize(static_cast<unsigned int>(setButtonTextSize));
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

	// details stuff

	float detailsHeaderTopMargin = 4.f;
	float detailsHeaderHeight = 27.f;
	float detailsTopMargin = detailsHeaderTopMargin + detailsHeaderHeight;
	unsigned int detailsHeaderTextSize = 18u;
	unsigned int detailsLabelTextSize = 14u;
	float detailsMiddleSpacing = 20.f;
	float detailsLineSpacing = 5.f;
	float detailsPicSideMargin = 40.f;
	detailRows = 7u;
	detailsPicSize = 96.f;
	detailsPicTop = detailsView.getSize().y * 0.5f - detailsPicSize * 0.5f;
	detailsPicLeft = detailsView.getSize().x - detailsPicSideMargin - detailsPicSize;
	detailsHeaderMiddle = detailsView.getSize().x * 0.5f;
	float detailsWidth = detailsPicLeft - detailsPicSideMargin * 0.5f;

	detailsMiddle = detailsWidth * 0.5f;
	detailsKeyMiddle = detailsWidth * 0.25f;
	detailsValueMiddle = detailsWidth * 0.75f;
	detailsLeftAnchor = detailsMiddle - detailsMiddleSpacing * 0.5f;
	detailsRightAnchor = detailsMiddle + detailsMiddleSpacing * 0.5f;

	detailsHeader = std::make_shared<tgui::Label>();
	detailsHeader->setAutoSize(true);
	detailsHeader->setTextSize(detailsHeaderTextSize);
	detailsHeader->setTextColor(sf::Color::White);
	detailsHeader->setText("");
	detailsGui.add(detailsHeader);
	detailsHeader->setPosition(detailsMiddle - detailsHeader->getSize().x * 0.5f, detailsHeaderTopMargin);

	for (unsigned int i = 0; i < detailRows; ++i)
	{
		std::vector<tgui::Label::Ptr> row;

		for (unsigned int j = 0; j < 3; ++j)
		{
			tgui::Label::Ptr detailsLabel = std::make_shared<tgui::Label>();
			detailsLabel->setAutoSize(true);
			detailsLabel->setTextSize(detailsLabelTextSize);
			detailsLabel->setTextColor(sf::Color::White);
			detailsLabel->setText("");
			detailsGui.add(detailsLabel);
			detailsLabel->setPosition((detailsKeyMiddle + j * detailsMiddle) - detailsLabel->getSize().x * 0.5f, detailsTopMargin + i * (detailsLabel->getSize().y + detailsLineSpacing));

			row.push_back(detailsLabel);
		}

		details.push_back(row);
	}

	sf::Texture& detailsTex = ResourceManager::getInstance().getTexture("key");
	detailsSprite.setTexture(detailsTex);
	detailsSprite.setPosition(detailsPicLeft, detailsPicTop);
	detailsSprite.setScale(sf::Vector2f(detailsPicSize / detailsTex.getSize().x, detailsPicSize / detailsTex.getSize().y));
}

void GameStateGame::changeSet(unsigned int numerator)
{
	float activeOpacity = 1.f;
	float inactiveOpacity = 0.7f;

	if (!inFight)
	{
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
		draggedFromEquipment = false;
	}

	if (eventType == MouseEvent::DRAGRELEASE)
	{
		if (draggedItem == nullptr)
		{
			dragging = false;
			return;
		}
	}

	if (pos.x < horSplitAbs && pos.y < verSplitAbs && !inFight) // inside map
	{
		if (eventType == MouseEvent::DRAGSTART)
		{
			draggedItem = nullptr;
			dragging = false;
		}
		else if (eventType == MouseEvent::CLICK)
		{
			std::shared_ptr<RenderableObject> retObj = map->getItemAtPixels(pos);
			
			if (retObj != nullptr)
			{
				updateDetails(DetailsBag(retObj));
			}
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
					OutputFormatter::chat(chatbox, "Could not drop " + draggedItem->getName(), sf::Color::White);
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
				draggedFromEquipment = true;
			}
		}
		else if (eventType == MouseEvent::CLICK)
		{
			std::shared_ptr<RenderableObject> retObj = player->getArmorItemAtPixels(pos);

			if (retObj != nullptr)
			{
				updateDetails(DetailsBag(retObj));
			}
		}
		else if (eventType == MouseEvent::DRAGRELEASE)
		{
			std::cout << "draggedItem: " << draggedItem << std::endl;

			std::shared_ptr<RenderableObject> oldDraggedItem = draggedItem;

			std::list<std::shared_ptr<RenderableObject>> retObjs = player->getEquipmentSet()->setItemAtPixels(pos, draggedItem, usePotionActive, fight);
			bool contains = false;

			std::cout << "  RETURNED: - " << retObjs.size() << std::endl;

			for (std::shared_ptr<RenderableObject> obj : retObjs)
			{
				if (obj == oldDraggedItem)
				{
					contains = true;
				}
				std::cout << obj << std::endl;
				draggedItem = obj;
				handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE);
			}

			if (!contains)
			{
				OutputFormatter::chat(chatbox, "Equipped " + oldDraggedItem->getName(), sf::Color::White);
			}

			dragging = false;
			draggedItem = nullptr;
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
			std::shared_ptr<RenderableObject> retObj = player->getInventoryItemAtPixels(pos);

			if (retObj != nullptr)
			{
				updateDetails(DetailsBag(retObj));
			}
		}
		else if (eventType == MouseEvent::DRAGRELEASE)
		{
			std::string oldDraggedItemName = draggedItem->getName();
			if ((draggedItem = player->putInInventory(draggedItem, false)) != nullptr)
			{
				handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE);
			}
			else if (draggedFromEquipment)
			{
				OutputFormatter::chat(chatbox, "Unequipped " + oldDraggedItemName, sf::Color::White);
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

void GameStateGame::updateDetails(DetailsBag& detailsBag)
{
	// empty all entries
	detailsHeader->setText("");
	for (unsigned int row = 0; row < detailRows; ++row)
	{
		for (unsigned int col = 0; col < 3; ++col)
		{
			details[row][col]->setText("");
		}
	}

	detailsHeader->setText(detailsBag.getName());

	std::cout << "detailsGrid: rows = " << details.size() << ", cols = " << details[0].size() << std::endl;
	std::cout << "detailsBag: rows = " << detailsBag.getDetails().size();

	if (detailsBag.getDetails().size() == 0)
	{
		std::cout << std::endl << "detialsBag was empty" << std::endl;
	}
	else
	{
		std::cout << ", cols = " << detailsBag.getDetails()[0].size() << std::endl;
	}
	
	for (unsigned int row = 0; row < detailsBag.getDetails().size(); ++row)
	{
		for (unsigned int col = 0; col < detailsBag.getDetails()[row].size(); ++col)
		{
			if (detailsBag.getDetails()[row][0] == "description") // print description over two cols and without key
			{
				if (col == 0)
				{
					details[row][col]->setText("");
				}
				else if (col == 1)
				{
					details[row][col]->setText(detailsBag.getDetails()[row][col]);
					details[row][col]->setPosition(detailsMiddle - details[row][col]->getSize().x * 0.5f, details[row][col]->getPosition().y);
				}
			}
			else // print normal key value pair
			{
				if (col == 0)
				{
					details[row][col]->setText(detailsBag.getDetails()[row][col]);
					details[row][col]->setPosition(detailsLeftAnchor - details[row][col]->getSize().x, details[row][col]->getPosition().y);

					details[row][col+1]->setText(":");
					details[row][col+1]->setPosition(detailsMiddle - details[row][col+1]->getSize().x * 0.5f, details[row][col+1]->getPosition().y);
				}
				else if (col == 1)
				{
					details[row][col+1]->setText(detailsBag.getDetails()[row][col]);
					details[row][col+1]->setPosition(detailsRightAnchor, details[row][col+1]->getPosition().y);
				}
			}
		}
	}

	detailsHeader->setPosition(detailsMiddle - detailsHeader->getSize().x * 0.5f, detailsHeader->getPosition().y);
	detailsSprite.setTexture(detailsBag.getDetailsPic());
}

void GameStateGame::startFight(std::shared_ptr<Player> player_, std::shared_ptr<Monster> monster_)
{
	std::cout << "about to start fight between " << player_->getName() << " and " << monster_->getName() << std::endl;
	inFight = true;

	fight.reset(new Fight(player_, monster_));
}

void GameStateGame::toggleAttackGui()
{
	if (!inAttackOptions)
	{
		fightGui.add(headButton);
		fightGui.add(torsoButton);
		fightGui.add(armsButton);
		fightGui.add(legsButton);
		inAttackOptions = true;
		return;
	}
	fightGui.remove(headButton);
	fightGui.remove(torsoButton);
	fightGui.remove(armsButton);
	fightGui.remove(legsButton);
	inAttackOptions = false;
}

void GameStateGame::hideAttackGui()
{
	if (inAttackOptions)
	{
		fightGui.remove(headButton);
		fightGui.remove(torsoButton);
		fightGui.remove(armsButton);
		fightGui.remove(legsButton);
		inAttackOptions = false;
	}
}

void GameStateGame::parry()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Trying to parry the Enemy", sf::Color::White);
}

void GameStateGame::usePotion()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Trying to use Potion", sf::Color::White);
}

void GameStateGame::toggleEquipment()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Changing Equipment", sf::Color::White);
}

void GameStateGame::attackHead()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Trying to attack the enemy's head", sf::Color::White);
}

void GameStateGame::attackTorso()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Trying to attack the enemy's torso", sf::Color::White);
}

void GameStateGame::attackArms()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Tring to attack the enemy's arms", sf::Color::White);
}

void GameStateGame::attackLegs()
{
	hideAttackGui();
	OutputFormatter::chat(chatbox, "Trying to attack the enemy's legs", sf::Color::White);
}

GameStateGame::~GameStateGame()
{
	delete map;
}