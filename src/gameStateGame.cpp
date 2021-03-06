#include <SFML/Graphics.hpp>

#include "gameStateGame.hpp"
#include "gameStatePauseMenu.hpp"
#include "gameStateEnd.hpp"
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

DetailsBag::DetailsBag(std::shared_ptr<RenderableObject> obj, bool playSound) : detailsPic(ResourceManager::getInstance().getTexture(obj->getName()))
{
	if (obj->getObjectType() == ObjectType::KEY)
	{
		name = "Key";
		addRow("description", "unlocks treasure chamber door");
		if (playSound)
		{
			ResourceManager::getInstance().getSound("key").play();
		}
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
			addRow("weapon type", EnumMapper::mapWeaponTypeName(weapon->getType()) + " handed");
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

			//std::cout << "description: " << description << std::endl;

			addRow("description", description);
			addRow("item class", EnumMapper::mapClassesName(potion->getItemClass()));
		}

		if (playSound)
		{
			ResourceManager::getInstance().getSound(item->getName()).play();
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
			addRow("kill bonus", /*OutputFormatter::shortFloat(monster->getKillBonus()) + "  " + */ EnumMapper::mapAttributeName(monster->getKillBonusType()));

			if (playSound)
			{
				ResourceManager::getInstance().getSound(monster->getName()).play();
			}
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
	topVerSplit = 0.07f;
	rightHorSplit = 0.815f;
	rightVerSplit = 0.5f;
	bottomVerSplit = 0.786f;
	bottomHorSplit = 0.5f; // relative to horSplit

	rightHorSplitAbs = static_cast<int>(rightHorSplit * size.x);
	topVerSplitAbs = static_cast<int>(topVerSplit * size.y);
	rightVerSplitAbs = static_cast<int>(rightVerSplit * size.y);
	bottomVerSplitAbs = static_cast<int>(bottomVerSplit * size.y);
	bottomHorSplitAbs = static_cast<int>(bottomHorSplit * rightHorSplit * size.x);

	float armorImageHeight = size.x - rightHorSplitAbs;
	
	// for aspect Ratio 16:9
	sf::Vector2f statsViewSize = sf::Vector2f(static_cast<float>(rightHorSplitAbs), static_cast<float>(topVerSplitAbs));
	sf::Vector2f possibleMapViewSize = sf::Vector2f(static_cast<float>(rightHorSplitAbs), static_cast<float>(bottomVerSplitAbs - topVerSplitAbs));
	viewportSize = possibleMapViewSize;
	sf::Vector2f fightViewSize = sf::Vector2f(static_cast<float>(rightHorSplitAbs), static_cast<float>(bottomVerSplitAbs - topVerSplitAbs));
	sf::Vector2f chatViewSize = sf::Vector2f(static_cast<float>(bottomHorSplitAbs), static_cast<float>(size.y - bottomVerSplitAbs));
	sf::Vector2f detailsViewSize = sf::Vector2f(static_cast<float>(bottomHorSplitAbs), static_cast<float>(size.y - bottomVerSplitAbs));
	sf::Vector2f armorViewSize = sf::Vector2f(static_cast<float>(size.x - rightHorSplitAbs), static_cast<float>(rightVerSplitAbs));
	sf::Vector2f inventoryViewSize = sf::Vector2f(static_cast<float>(size.x - rightHorSplitAbs), static_cast<float>(size.y - rightVerSplitAbs));

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

	statsView.setViewport(sf::FloatRect(0.f, 0.f, rightHorSplit, topVerSplit));
	fightView.setViewport(sf::FloatRect(0.f, topVerSplit, rightHorSplit, bottomVerSplit - topVerSplit));
	mapView.setViewport(sf::FloatRect(left / 2, topVerSplit +  top / 2, viewportSize.x / size.x, viewportSize.y / size.y));
	armorView.setViewport(sf::FloatRect(rightHorSplit, 0.f, (1.f - rightHorSplit), rightVerSplit));
	chatView.setViewport(sf::FloatRect(0.f, bottomVerSplit, rightHorSplit * bottomHorSplit, (1.f - bottomVerSplit)));
	detailsView.setViewport(sf::FloatRect(rightHorSplit * bottomHorSplit, bottomVerSplit, (rightHorSplit - rightHorSplit * bottomHorSplit), (1.f - bottomVerSplit)));
	inventoryView.setViewport(sf::FloatRect(rightHorSplit, rightVerSplit, (1.f - rightHorSplit), (1.f - rightVerSplit)));
	completeView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	statsView.setSize(statsViewSize);
	fightView.setSize(fightViewSize);
	mapView.setSize(viewportSize);
	armorView.setSize(armorViewSize);
	chatView.setSize(chatViewSize);
	detailsView.setSize(detailsViewSize);
	inventoryView.setSize(inventoryViewSize);
	completeView.setSize(size);
	statsView.setCenter(statsViewSize * 0.5f);
	mapView.setCenter(viewportSize * 0.5f);
	fightView.setCenter(fightViewSize * 0.5f);
	armorView.setCenter(armorViewSize * 0.5f);
	chatView.setCenter(chatViewSize * 0.5f);
	detailsView.setCenter(detailsViewSize * 0.5f);
	inventoryView.setCenter(inventoryViewSize * 0.5f);
	completeView.setCenter(size * 0.5f);

	initialMapCenter = mapView.getCenter();

	// for camera center adaption
	borderMargin.x = static_cast<float>(settings->tileSize * (settings->ROOM_WIDTH + 1));
	borderMargin.y = static_cast<float>(settings->tileSize * (settings->ROOM_HEIGHT + 1));
	minTop = borderMargin.y;
	maxBottom = mapSize.y - borderMargin.y;
	minLeft = borderMargin.x;
	maxRight = mapSize.x - borderMargin.x;

	armorSprite.setTexture(ResourceManager::getInstance().getTexture("armorBackground"));
	potionSprite.setTexture(ResourceManager::getInstance().getTexture("potionBar"));
	hpSprite.setTexture(ResourceManager::getInstance().getTexture("hp"));
	accuracySprite.setTexture(ResourceManager::getInstance().getTexture("accuracy"));
	strengthSprite.setTexture(ResourceManager::getInstance().getTexture("strength"));
	speedSprite.setTexture(ResourceManager::getInstance().getTexture("speed"));
	playerSprite.setTexture(ResourceManager::getInstance().getTexture("player_big"));

	player.reset(new Player("player", 50.f, 25.f, 25.f, 25.f, settings->playerName, static_cast<float>(settings->tileSize), settings->maxInventorySize, sf::Vector2f(armorLeftOffset, armorTopOffset), sf::Vector2f(potionLeftOffset, potionTopOffset)));
	player->setSize(settings->tileSize, settings->tileSize);

	map = new Map(game);
	map->init(player);

	theme = std::make_shared<tgui::Theme>(settings->IMAGE_PATH + "widgets/Black.txt");

	loadGui();

	player->init(map, settings->tileSize, chatbox);
	player->setEquipmentFists(game.getPrototypeStorage()->weaponFactory->create("Fists"), game.getPrototypeStorage()->weaponFactory->create("Fists"));

	OutputFormatter::chat(chatbox, "Hello " + player->getPlayerName() + ", welcome to the Dungeon!", sf::Color::White);

	// test fill set
#if 0
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
#endif
}

void GameStateGame::draw(const float deltaTime)
{
	game.window.clear(sf::Color::Black);

	game.window.setView(mapView);
	if (!inFight)
	{
		map->draw(game.window, deltaTime);
		player->draw(game.window, deltaTime);
	}

	if (inFight)
	{
		game.window.setView(fightView);
		game.window.clear(sf::Color::White);
		game.window.draw(fightBackground);
		game.window.draw(enemySprite);
		game.window.draw(playerSprite);
		fightGui.draw();
	}

	game.window.setView(statsView);
	statsGui.draw();
	game.window.draw(strengthSprite);
	game.window.draw(speedSprite);
	game.window.draw(accuracySprite);
	game.window.draw(hpSprite);
	
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
	bool updateStatsAndDetails = false;

	statsUpdateAccumulator += deltaTime;

	if (triggeredEnd)
	{
		endAccumulator += deltaTime;
	}

	if (endAccumulator > endSpan && triggeredEnd)
	{
		game.changeState(std::shared_ptr<GameState>(new GameStateEnd(game, victory)));
	}

	if (statsUpdateAccumulator > statsUpdateSpan)
	{
		statsUpdateAccumulator = 0;
		updateStatsAndDetails = true;
	}

	if (!inFight)
	{
		player->update(deltaTime); // move player
	}
	else {

		if (fight->isFightEnded())
		{
			std::shared_ptr<Creature> loser = fight->getLoser();

			fightStageAccumulator += deltaTime;
			if (fightStageAccumulator >= 1.2f * fightStageSpan)
			{
				endFight(loser);
				fightStageAccumulator = 0;
			}
		}

		if (!fight->isFightEnded() && fight->getLoser() != nullptr)
		{
			fightStageAccumulator += deltaTime;
			if (fightStageAccumulator >= fightStageSpan)
			{
				fight->end();
				fightStageAccumulator = 0;
			}
		}

#if 1
		else
		{
			if (fight->getActiveRound() == 2u) // only automatically enter 2nd round, 1st round is triggered by buttons in gui
			{
				fightStageAccumulator += deltaTime;

				if (fightStageAccumulator >= fightStageSpan)
				{
					//std::cout << "triggered second round from update" << std::endl;
					fight->fightRound(fight->getPlayerAttackType(), 2u);
					fightStageAccumulator = 0;
				}
			}

			if (showingEnemyDetails && updateStatsAndDetails)
			{
				updateDetails(DetailsBag(fight->getEnemy(), false), true);
			}
		}
#endif
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
			sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);

			mousePos.x = static_cast<int>(settings->widthDownScaleFactor * mousePos.x);
			mousePos.y = static_cast<int>(settings->heightDownScaleFactor * mousePos.y);

			draggedItem->setCenteredPosition(mousePos);
		}
	}

	if (updateStatsAndDetails)
	{
		updateStats();
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
		chatGui.handleEvent(event);
		if (inFight)
		{
			fightGui.handleEvent(event);
		}
		armorGui.handleEvent(event);

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
					game.changeMusic("game", 0.7f, 1.5f, 0.5f, true);

					fight.reset();
					inFight = false;
				}
				else
				{
					game.window.close();
				}
			}
			else if (event.key.code == sf::Keyboard::P) // debug
			{
				usePotionActive = !usePotionActive;
			}
			else if (event.key.code == sf::Keyboard::L)
			{
				triggeredEnd = true;
				victory = false;
				game.music.setLoop(false);
				game.changeMusic("defeat", 0.7f, 0.0f, 0.5f, false);
				endSpan = 1.0f;
			}
			else if (event.key.code == sf::Keyboard::V)
			{
				triggeredEnd = true;
				victory = true;
				game.music.setLoop(false);
				game.changeMusic("victory", 0.7f, 0.0f, 0.5f, false);
				endSpan = 1.0f;
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				if (usePotionActive)
				{
					usePotionActive = false;
				}
				else if (inAttackOptions)
				{
					hideAttackGui();
				}
				else
				{
					game.changeMusic("mainMenu", 0.7f, 1.0f, 0.f);
					pauseGame();
				}
			}
			else
			{
				std::shared_ptr<Monster> enemy = nullptr;

				if ((enemy = player->handleInput(event, draggedItem)) != nullptr)
				{
					startFight(player, enemy);
				}
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
	statsGui.removeAllWidgets();
	statsGui.setWindow(game.window);

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
	statsGui.setFont("res/fonts/DejaVuSans.ttf");
	chatGui.setFont("res/fonts/DejaVuSans.ttf");
	detailsGui.setFont("res/fonts/DejaVuSans.ttf");
	armorGui.setFont("res/fonts/DejaVuSans.ttf");
	inventoryGui.setFont("res/fonts/DejaVuSans.ttf");
	fightGui.setFont("res/fonts/DejaVuSans.ttf");

	// gui loading

	// stats
	statsbox = theme->load("ChatBox");
	statsbox->setSize(static_cast<float>(rightHorSplitAbs), static_cast<float>(topVerSplitAbs));
	statsbox->setPosition(0, 0);
	statsGui.add(statsbox, "stats");

	unsigned int playerNameTextSize = 18u;
	float playerNameLeft = 20.f;
	statsMarginRight = 10.f;
	statsSpacing = 5.f;
	float rightOffsetSum = rightHorSplitAbs - statsMarginRight;
	statsPicSize = 36.f;

	sf::Texture& strengthTex = ResourceManager::getInstance().getTexture("strength");
	sf::Texture& speedTex = ResourceManager::getInstance().getTexture("speed");
	sf::Texture& accuracyTex = ResourceManager::getInstance().getTexture("accuracy");
	sf::Texture& hpTex = ResourceManager::getInstance().getTexture("hp");

	playerNameLabel = std::make_shared<tgui::Label>();
	playerNameLabel->setAutoSize(true);
	playerNameLabel->setTextSize(playerNameTextSize);
	playerNameLabel->setTextColor(sf::Color::White);
	playerNameLabel->setText(player->getPlayerName());
	statsGui.add(playerNameLabel);
	playerNameLabel->setPosition(playerNameLeft, (topVerSplitAbs - playerNameLabel->getSize().y) * 0.5f);

	strengthLabel = std::make_shared<tgui::Label>();
	strengthLabel->setAutoSize(true);
	strengthLabel->setTextSize(playerNameTextSize);
	strengthLabel->setTextColor(sf::Color::White);
	strengthLabel->setText(OutputFormatter::shortFloat(player->strength));
	statsGui.add(strengthLabel);
	rightOffsetSum -= (strengthLabel->getSize().x + 2 * statsSpacing);
	strengthLabel->setPosition(rightOffsetSum, (topVerSplitAbs - strengthLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	strengthSprite.setScale(sf::Vector2f(statsPicSize / strengthTex.getSize().x, statsPicSize / strengthTex.getSize().y));
	strengthSprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	speedLabel = std::make_shared<tgui::Label>();
	speedLabel->setAutoSize(true);
	speedLabel->setTextSize(playerNameTextSize);
	speedLabel->setTextColor(sf::Color::White);
	speedLabel->setText(OutputFormatter::shortFloat(player->speed));
	statsGui.add(speedLabel);
	rightOffsetSum -= (speedLabel->getSize().x + 2 * statsSpacing);
	speedLabel->setPosition(rightOffsetSum, (topVerSplitAbs - speedLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	speedSprite.setScale(sf::Vector2f(statsPicSize / speedTex.getSize().x, statsPicSize / speedTex.getSize().y));
	speedSprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	accuracyLabel = std::make_shared<tgui::Label>();
	accuracyLabel->setAutoSize(true);
	accuracyLabel->setTextSize(playerNameTextSize);
	accuracyLabel->setTextColor(sf::Color::White);
	accuracyLabel->setText(OutputFormatter::shortFloat(player->accuracy));
	statsGui.add(accuracyLabel);
	rightOffsetSum -= (accuracyLabel->getSize().x + 2 * statsSpacing);
	accuracyLabel->setPosition(rightOffsetSum, (topVerSplitAbs - accuracyLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	accuracySprite.setScale(sf::Vector2f(statsPicSize / accuracyTex.getSize().x, statsPicSize / accuracyTex.getSize().y));
	accuracySprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	hpLabel = std::make_shared<tgui::Label>();
	hpLabel->setAutoSize(true);
	hpLabel->setTextSize(playerNameTextSize);
	hpLabel->setTextColor(sf::Color::White);
	hpLabel->setText(OutputFormatter::shortFloat(player->hp));
	statsGui.add(hpLabel);
	rightOffsetSum -= (hpLabel->getSize().x + 2 * statsSpacing);
	hpLabel->setPosition(rightOffsetSum, (topVerSplitAbs - hpLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	hpSprite.setScale(sf::Vector2f(statsPicSize / hpTex.getSize().x, statsPicSize / hpTex.getSize().y));
	hpSprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	statsGui.setView(statsView);

	// chatbox
	chatbox = theme->load("ChatBox");
	chatbox->setSize(static_cast<float>(bottomHorSplitAbs), static_cast<float>(size.y - bottomVerSplitAbs));
	chatbox->setTextSize(17u);
	chatbox->setPosition(0, 0);
	chatbox->setLineLimit(7);
	chatbox->setLinesStartFromTop();
	chatGui.add(chatbox, "log");

	player->setChatbox(chatbox);

	chatGui.setView(chatView);

	detailsbox = theme->load("ChatBox");
	detailsbox->setSize(static_cast<float>(bottomHorSplitAbs), static_cast<float>(size.y - bottomVerSplitAbs));
	detailsbox->setPosition(0, 0);
	detailsGui.add(detailsbox, "details");

	detailsGui.setView(detailsView);

	inventorybox = theme->load("ChatBox");
	inventorybox->setSize(static_cast<float>(size.x - rightHorSplitAbs), static_cast<float>(size.y - rightVerSplitAbs));
	inventorybox->setPosition(0, 0);
	inventoryGui.add(inventorybox, "inventory");

	inventoryGui.setView(inventoryView);

	armorbox = theme->load("ChatBox");
	armorbox->setSize(static_cast<float>(size.x - rightHorSplitAbs), static_cast<float>(rightVerSplitAbs));
	armorbox->setPosition(0, 0);
	armorGui.add(armorbox, "armor");

	/* FIGHT STUFF */

	float spacer = 15.f * settings->widthScaleFactor;
	float buttonWidth = (rightHorSplitAbs - spacer) / 4.f;
	float buttonHeight = bottomVerSplitAbs / 8.f;
	float buttonCount = 4.f;
	float spacePerButton = spacer / buttonCount;
	float buttonWithSpacer = buttonWidth + spacePerButton;
	float heightSpacer = 6.f;
	float healthBarWidth = 300.f;
	float healthBarHeight = 15.f;
	float spriteSpacer = 200.f;
	float spriteSize = 200.f * 1.5f;
	float healthBarTopSpace = 10.f;
	unsigned int grayScaleComponent = 100;

	fightBackground = sf::RectangleShape(sf::Vector2f(static_cast<float>(rightHorSplitAbs), static_cast<float>(bottomVerSplitAbs - topVerSplitAbs)));
	fightBackground.setFillColor(sf::Color(grayScaleComponent, grayScaleComponent, grayScaleComponent));

	fightGui.setView(fightView);

	attackButton = theme->load("Button");
	attackButton->setText("Attack");
	attackButton->setOpacity(1.f);
	attackButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	attackButton->connect("pressed", [&](){ toggleAttackGui(); });
	attackButton->setSize(buttonWidth, buttonHeight);
	attackButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer);
	fightGui.add(attackButton);

	parryButton = theme->load("Button");
	parryButton->setText("Force Parry");
	parryButton->setOpacity(1.f);
	parryButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	parryButton->connect("pressed", [&](){ parry(); });
	parryButton->setSize(buttonWidth, buttonHeight);
	parryButton->setPosition(1.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer);
	fightGui.add(parryButton);

	potionButton = theme->load("Button");
	potionButton->setText("Use Potion");
	potionButton->setOpacity(1.f);
	potionButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	potionButton->connect("pressed", [&](){ usePotion(); });
	potionButton->setSize(buttonWidth, buttonHeight);
	potionButton->setPosition(2.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer);
	fightGui.add(potionButton);

	equipmentSetButton = theme->load("Button");
	equipmentSetButton->setText("Change Equipment");
	equipmentSetButton->setOpacity(1.f);
	equipmentSetButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	equipmentSetButton->connect("pressed", [&](){ toggleEquipment(); });
	equipmentSetButton->setSize(buttonWidth, buttonHeight);
	equipmentSetButton->setPosition(3.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer);
	fightGui.add(equipmentSetButton);

	/* Attack Submenu */
	headButton = theme->load("Button");
	headButton->setText("Head");
	headButton->setOpacity(1.f);
	headButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	headButton->connect("pressed", [&](){ attackHead(); });
	headButton->setSize(buttonWidth, buttonHeight);
	headButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer - 1.f * buttonHeight);

	torsoButton = theme->load("Button");
	torsoButton->setText("Torso");
	torsoButton->setOpacity(1.f);
	torsoButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	torsoButton->connect("pressed", [&](){ attackTorso(); });
	torsoButton->setSize(buttonWidth, buttonHeight);
	torsoButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer - 2.f * buttonHeight);

	armsButton = theme->load("Button");
	armsButton->setText("Arms");
	armsButton->setOpacity(1.f);
	armsButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	armsButton->connect("pressed", [&](){ attackArms(); });
	armsButton->setSize(buttonWidth, buttonHeight);
	armsButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer - 3.f * buttonHeight);

	legsButton = theme->load("Button");
	legsButton->setText("Legs");
	legsButton->setOpacity(1.f);
	legsButton->setTextSize(static_cast<unsigned int>(0.8f * settings->buttonTextSize));
	legsButton->connect("pressed", [&](){ attackLegs(); });
	legsButton->setSize(buttonWidth, buttonHeight);
	legsButton->setPosition(0.f * buttonWithSpacer + spacePerButton / 2.f, bottomVerSplitAbs - topVerSplitAbs - buttonHeight - heightSpacer - 4.f * buttonHeight);

	playerNameFightLabel = std::make_shared<tgui::Label>();
	playerNameFightLabel->setAutoSize(true);
	playerNameFightLabel->setTextSize(static_cast<unsigned int>(25.f));
	playerNameFightLabel->setTextColor(sf::Color::Black);
	playerNameFightLabel->setText(settings->playerName);
	fightGui.add(playerNameFightLabel);
	playerNameFightLabel->setPosition(spacer, spacer);

	enemyNameFightLabel = std::make_shared<tgui::Label>();
	enemyNameFightLabel->setAutoSize(true);
	enemyNameFightLabel->setTextSize(static_cast<unsigned int>(25.f));
	enemyNameFightLabel->setTextColor(sf::Color::Black);
	enemyNameFightLabel->setText("Enemy");
	fightGui.add(enemyNameFightLabel);
	enemyNameFightLabel->setPosition(rightHorSplitAbs - spriteSize - spacer, spacer);

	playerHealthBar = std::make_shared<tgui::ProgressBar>();
	playerHealthBar->setPosition(spacer, playerNameFightLabel->getPosition().y + playerNameFightLabel->getTextSize() + healthBarTopSpace);
	playerHealthBar->setSize(healthBarWidth, healthBarHeight);
	playerHealthBar->setMinimum(0u);
	playerHealthBar->setMaximum(100u);
	playerHealthBar->setValue(50u);
	fightGui.add(playerHealthBar);

	enemyHealthBar = std::make_shared<tgui::ProgressBar>();
	enemyHealthBar->setPosition(rightHorSplitAbs - spriteSize - spacer, playerNameFightLabel->getPosition().y + playerNameFightLabel->getTextSize() + healthBarTopSpace);
	enemyHealthBar->setSize(healthBarWidth, healthBarHeight);
	enemyHealthBar->setMinimum(0u);
	enemyHealthBar->setMaximum(100u);
	enemyHealthBar->setValue(50u);
	fightGui.add(enemyHealthBar);

	playerSprite.setPosition(spacer, attackButton->getPosition().y - spacer - spriteSize);
	playerSprite.setScale(sf::Vector2f(1.5f, 1.5f));

	//enemySprite.setPosition(bottomVerSplitAbs + 6.f * spacer, bottomVerSplitAbs - topVerSplitAbs - spriteSpacer);
	enemySprite.setPosition(rightHorSplitAbs - spriteSize - spacer, attackButton->getPosition().y - spacer - spriteSize);
	enemySprite.setScale(sf::Vector2f(1.5f, 1.5f));

	/* FIGHT STUFF END */

	float armorButtonsTopMargin = 10.f;
	float armorButtonsLeftMargin = 8.f;
	float spacing = 2.f;
	sf::Vector2f setButtonSize((size.x - rightHorSplitAbs) * 0.5f - armorButtonsLeftMargin - spacing, 35);
	unsigned int setButtonTextSize = 18u;

	set1Button = theme->load("Button");
	set1Button->setText("Set 1");
	set1Button->setPosition(armorButtonsLeftMargin, armorButtonsTopMargin);
	set1Button->setSize(setButtonSize.x, setButtonSize.y);
	set1Button->setTextSize(static_cast<unsigned int>(setButtonTextSize));
	set1Button->connect("pressed", [&](){ changeSet(true, 1); });
	armorGui.add(set1Button);

	set2Button = theme->load("Button");
	set2Button->setText("Set 2");
	set2Button->setPosition(armorButtonsLeftMargin + setButtonSize.x + 2 * spacing, armorButtonsTopMargin);
	set2Button->setSize(setButtonSize.x, setButtonSize.y);
	set2Button->setTextSize(static_cast<unsigned int>(setButtonTextSize));
	set2Button->connect("pressed", [&](){ changeSet(true, 2); });
	armorGui.add(set2Button);

	changeSet(false, player->getEquipmentSet()->getNumerator());

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

	player->setEquipmentGeometry(sf::Vector2f(armorLeftOffset, armorTopOffset), sf::Vector2f(potionLeftOffset, potionTopOffset), armorDims, potionDims, settings->tileSize);

	armorGui.setView(armorView);

	// details stuff

	float detailsHeaderTopMargin = 2.f;
	float detailsHeaderHeight = 27.f;
	float detailsTopMargin = detailsHeaderTopMargin + detailsHeaderHeight;
	unsigned int detailsHeaderTextSize = 18u;
	unsigned int detailsLabelTextSize = 14u;
	float detailsMiddleSpacing = 20.f;
	float detailsLineSpacing = 4.f;
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

void GameStateGame::changeSet(bool playSound)
{
	unsigned int activeNumerator = player->getEquipmentSet()->getNumerator();

	if (activeNumerator == 1u)
	{
		changeSet(playSound, 2u);
	}
	else if (activeNumerator == 2u)
	{
		changeSet(playSound, 1u);
	}
}

void GameStateGame::changeSet(bool playSound, unsigned int numerator)
{
	float activeOpacity = 1.f;
	float inactiveOpacity = 0.7f;

	if (!inFight || choseChangeSet)
	{
		if (playSound)
		{
			ResourceManager::getInstance().getSound("changeSet").play();
		}
		
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

		choseChangeSet = false;
	}
	else 
	{
		if (playSound)
		{
			ResourceManager::getInstance().getSound("error").play();
		}
	}
}

void GameStateGame::handleMouseEvent(sf::Vector2i pos_, MouseEvent::Enum eventType, bool noScale)
{
	// scale mouse position
	sf::Vector2i pos;

	/*std::cout << std::endl;
	std::cout << "noScale? " << noScale << std::endl;
	std::cout << "mouseEvent raw: pos = " << pos_.x << ", " << pos_.y << ", type: " << eventType << std::endl;*/

	if (noScale)
	{
		//std::cout << "mouseEvent unscaled: pos = " << pos.x << ", " << pos.y << ", type: " << eventType << std::endl;
		pos = pos_;
	}
	else
	{
		pos.x = static_cast<int>(pos_.x * settings->widthDownScaleFactor);
		pos.y = static_cast<int>(pos_.y * settings->heightDownScaleFactor);
	}

	//std::cout << "mouseEvent scaled: pos = " << pos.x << ", " << pos.y << ", type: " << eventType << std::endl;

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

	if (pos.x < rightHorSplitAbs && pos.y < bottomVerSplitAbs && pos.y >= topVerSplitAbs) // inside map
	{

		sf::Vector2i relPos;
		sf::Vector2f mapViewCenter = mapView.getCenter();
		sf::Vector2f mapCenter = sf::Vector2f(rightHorSplitAbs * 0.5f, (bottomVerSplitAbs - topVerSplitAbs) * 0.5f);

		relPos.x = pos.x/* + static_cast<int>(mapCenter.x - mapViewCenter.x)*/;
		relPos.y = pos.y - topVerSplitAbs /*+ static_cast<int>(mapCenter.y - mapViewCenter.y)*/;

		if (!inFight)
		{
			/*std::cout << "mapViewCenter: x = " << mapViewCenter.x << ", y = " << mapViewCenter.y << std::endl;
			std::cout << "mapCenter: x = " << mapCenter.x << ", y = " << mapCenter.y << std::endl;
*/
			//std::cout << "before relPos.x = " << relPos.x << ", relPos.y = " << relPos.y << std::endl;

			relPos.x = relPos.x - static_cast<int>(mapCenter.x - mapViewCenter.x);
			relPos.y = relPos.y - static_cast<int>(mapCenter.y - mapViewCenter.y);

			/*std::cout << "after relPos.x = " << relPos.x << ", relPos.y = " << relPos.y << std::endl;*/

			if (eventType == MouseEvent::DRAGSTART)
			{
				draggedItem = nullptr;
				dragging = false;
			}
			else if (eventType == MouseEvent::CLICK)
			{
				std::shared_ptr<RenderableObject> retObj = map->getItemAtPixels(relPos);

				if (retObj != nullptr)
				{
					if (retObj->getObjectType() == ObjectType::ITEM || retObj->getObjectType() == ObjectType::CREATURE || retObj->getObjectType() == ObjectType::KEY)
					{
						updateDetails(DetailsBag(retObj));
					}
				}
			}
			else if (eventType == MouseEvent::DRAGRELEASE)
			{
				if (draggedItem != nullptr)
				{
					Point facingPoint = player->getPlayerPosition().getDirPoint(player->getFacingDir());
					if (map->getOverlayObject(facingPoint) == nullptr)
					{
						ResourceManager::getInstance().getSound("dropItem").play();
						OutputFormatter::chat(chatbox, "Dropped " + draggedItem->getName(), sf::Color::White);
						map->setOverlayObject(facingPoint, draggedItem); // place dragged item on map if there is a free tile in front of the player
						draggedItem = nullptr;
					}
					else
					{
						ResourceManager::getInstance().getSound("error").play();
						OutputFormatter::chat(chatbox, "Could not drop " + draggedItem->getName(), sf::Color::White);
						handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE, true); // return dragged item to where it came from if there is no free tile in front of the player
					}
				}
				dragging = false;
			}
		}
		else { // clicked into fight window

			// hide attack menu when clicking somewhere different than the button, need to consume event
			if (!pressedAttackMainMenuButton)
			{
				hideAttackGui();
			}

			float left = enemySprite.getGlobalBounds().left;
			float top = enemySprite.getGlobalBounds().top;
			float right = (left + enemySprite.getGlobalBounds().width);
			float bottom = (top + enemySprite.getGlobalBounds().height);

			/*std::cout << "clicked in fight - relPos: x = " << relPos.x << ", y = " << relPos.y << std::endl;
			std::cout << "clicked in fight - enemySprite: left = " << left << ", top = " << top << ", right = " << right << ", bottom = " << bottom << std::endl;*/

			// show monster details 
			if (relPos.x >= left && relPos.y >= top && relPos.x < right && relPos.y < bottom)
			{
				// std::cout << "showing enemy details for " << fight->getEnemy()->getName() << std::endl;
				ResourceManager::getInstance().getSound(fight->getEnemy()->getSoundName()).play();
				updateDetails(DetailsBag(fight->getEnemy(), false));
				showingEnemyDetails = true;
			}

			pressedAttackMainMenuButton = false;
		}
	}
	else if (pos.x > rightHorSplitAbs && pos.y < rightVerSplitAbs) // inside armor
	{
		sf::Vector2i relPos;

		relPos.x = pos.x - rightHorSplitAbs;
		relPos.y = pos.y;

		if (eventType == MouseEvent::DRAGSTART)
		{
			if (!inFight || usePotionActive)
			{
				draggedItem = player->getArmorItemAtPixels(relPos, true, usePotionActive);

				//std::cout << "draggedItem: " << draggedItem << std::endl;

				if (draggedItem != nullptr)
				{
					draggedItem->setSize(settings->tileSize, settings->tileSize);
					draggedFromEquipment = true;
				}
			}
		}
		else if (eventType == MouseEvent::CLICK)
		{
			std::shared_ptr<RenderableObject> retObj = player->getArmorItemAtPixels(relPos);

			if (retObj != nullptr)
			{
				updateDetails(DetailsBag(retObj));
			}
		}
		else if (eventType == MouseEvent::DRAGRELEASE)
		{
			//std::cout << "draggedItem: " << draggedItem << std::endl;

			std::shared_ptr<RenderableObject> oldDraggedItem = draggedItem;

			std::list<std::shared_ptr<RenderableObject>> retObjs = player->getEquipmentSet()->setItemAtPixels(relPos, draggedItem, usePotionActive, fight, draggedFromEquipment);
			bool contains = false;

			//std::cout << "  RETURNED: - " << retObjs.size() << std::endl;

			for (std::shared_ptr<RenderableObject> obj : retObjs)
			{
				//std::cout << "       " << obj->getName() << std::endl;
				if (obj == oldDraggedItem)
				{
					contains = true;
				}
				//std::cout << obj << std::endl;
				draggedItem = obj;
				if (!draggedFromEquipment)
				{
					OutputFormatter::chat(chatbox, "Unequipped " + obj->getName(), sf::Color::White);
				}
				handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE, true);
			}

			if (!contains)
			{
				if (!usePotionActive)
				{
					OutputFormatter::chat(chatbox, "Equipped " + oldDraggedItem->getName(), sf::Color::White);
					ResourceManager::getInstance().getSound(oldDraggedItem->getName()).play();
				}
				else
				{
					usePotionActive = false;
				}
			}

			dragging = false;
			draggedItem = nullptr;
		}
	}
	else if (pos.x > rightHorSplitAbs && pos.y >= rightVerSplitAbs) // inside inventory
	{
		sf::Vector2i relPos;

		relPos.x = pos.x - rightHorSplitAbs;
		relPos.y = pos.y - rightVerSplitAbs;

		if (eventType == MouseEvent::DRAGSTART && !inFight)
		{
			draggedItem = player->getInventoryItemAtPixels(relPos, true);

			if (draggedItem != nullptr)
			{
				draggedItem->setSize(settings->tileSize, settings->tileSize);
			}
		}
		else if(eventType == MouseEvent::CLICK)
		{
			std::shared_ptr<RenderableObject> retObj = player->getInventoryItemAtPixels(relPos);

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
				handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE, true);
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
			ResourceManager::getInstance().getSound("error").play();
			handleMouseEvent(dragStartPos, MouseEvent::DRAGRELEASE, true);
			dragging = false;
		}
	}

	//std::cout << "End of Mouse Handling: draggedItem = " << draggedItem << ", primaryWeapon = " << player->getEquipmentSet()->getPrimaryWeapon() << ", secondaryWEapon = " << player->getEquipmentSet()->getSecondaryWeapon() << std::endl;
}

void GameStateGame::updateDetails(DetailsBag& detailsBag, bool showingEnemyDetails_)
{
	if (!showingEnemyDetails_)
	{
		showingEnemyDetails = false;
	}

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

void GameStateGame::updateStats()
{
	float rightOffsetSum = rightHorSplitAbs - statsMarginRight;

	sf::Texture& strengthTex = ResourceManager::getInstance().getTexture("strength");
	sf::Texture& speedTex = ResourceManager::getInstance().getTexture("speed");
	sf::Texture& accuracyTex = ResourceManager::getInstance().getTexture("accuracy");
	sf::Texture& hpTex = ResourceManager::getInstance().getTexture("hp");

	strengthLabel->setText(OutputFormatter::shortFloat(player->strength));
	rightOffsetSum -= (strengthLabel->getSize().x + 2 * statsSpacing);
	strengthLabel->setPosition(rightOffsetSum, (topVerSplitAbs - strengthLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	strengthSprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	speedLabel->setText(OutputFormatter::shortFloat(player->speed));
	rightOffsetSum -= (speedLabel->getSize().x + 2 * statsSpacing);
	speedLabel->setPosition(rightOffsetSum, (topVerSplitAbs - speedLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	speedSprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	accuracyLabel->setText(OutputFormatter::shortFloat(player->accuracy));
	rightOffsetSum -= (accuracyLabel->getSize().x + 2 * statsSpacing);
	accuracyLabel->setPosition(rightOffsetSum, (topVerSplitAbs - accuracyLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	accuracySprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	hpLabel->setText(OutputFormatter::shortFloat(player->hp));
	rightOffsetSum -= (hpLabel->getSize().x + 2 * statsSpacing);
	hpLabel->setPosition(rightOffsetSum, (topVerSplitAbs - hpLabel->getSize().y) * 0.5f);

	rightOffsetSum -= (statsPicSize + statsSpacing);
	hpSprite.setPosition(sf::Vector2f(rightOffsetSum, (topVerSplitAbs - statsPicSize) * 0.5f));

	if (inFight)
	{
		enemyHealthBar->setValue(static_cast<unsigned int>(fight->getEnemy()->hp / fight->getEnemy()->getOrHP() * 100));
		playerHealthBar->setValue(static_cast<unsigned int>(player->hp / player->getOrHP() * 100));
	}
}

void GameStateGame::startFight(std::shared_ptr<Player> player_, std::shared_ptr<Monster> monster_)
{
	//std::cout << "about to start fight between " << player_->getName() << " and " << monster_->getName() << std::endl;
	inFight = true;

	enemySprite.setTexture(ResourceManager::getInstance().getTexture(monster_->getName() + "_big"));
	enemyNameFightLabel->setText(monster_->getName());
	game.changeMusic("fight", 0.7f, 0.0f, 0.0f);
	fight.reset(new Fight(player_, monster_, game.getPrototypeStorage(), chatbox));

	OutputFormatter::chat(chatbox, "Started fight against " + fight->getEnemy()->getName(), sf::Color::White);
	ResourceManager::getInstance().getSound(fight->getEnemy()->getSoundName()).play();
	updateDetails(DetailsBag(fight->getEnemy(), false));
	updateStats();
	showingEnemyDetails = true;
}

void GameStateGame::endFight(std::shared_ptr<Creature> loser)
{
	if (loser->getCreatureType() == CreatureType::PLAYER)
	{
		if (fight->getEnemy()->getLevel() == DifficultyLevel::HARD)
		{
			triggeredEnd = true;
			victory = false;
			game.music.setLoop(false);
			game.changeMusic("defeat", 0.7f, 0.0f, 0.5f, false);
			endSpan = 1.0f;
			return;
		}

		player->resetOriginals();
		fight->getEnemy()->resetOriginals();
		player->setPlayerPosition(map->getInitialPlayerPosition());
		mapView.setCenter(initialMapCenter.x, initialMapCenter.y);
	}
	else
	{
		if (fight->getEnemy()->getLevel() == DifficultyLevel::HARD)
		{
			triggeredEnd = true;
			victory = true;
			game.music.setLoop(false);
			game.changeMusic("victory", 0.7f, 0.0f, 0.5f, false);
			endSpan = 1.0f;
			return;
		}

		fight->attributeBonusForWinner();
		player->resetOriginals();
		Point facingPoint = player->getPlayerPosition().getDirPoint(player->getFacingDir());
		map->setOverlayObject(facingPoint, nullptr);
	}

	game.changeMusic("game", 0.7f, 1.0f, 0.5f, true);

	updateStats();

	inFight = false;
	fightStageAccumulator = 0;
}

void GameStateGame::toggleAttackGui()
{
	pressedAttackMainMenuButton = true;
	if (interactionPermitted) {
		ResourceManager::getInstance().getSound("buttonClick").play();
		usePotionActive = false;
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
	if (interactionPermitted)
	{
		ResourceManager::getInstance().getSound("buttonClick").play();
		usePotionActive = false;
		fight->fightRound(Attacks::PARRY, 1u);
	}
}

void GameStateGame::usePotion()
{
	if (interactionPermitted)
	{
		ResourceManager::getInstance().getSound("buttonClick").play();
		usePotionActive = true;
	}
}

void GameStateGame::toggleEquipment()
{
	if (interactionPermitted)
	{
		usePotionActive = false;
		choseChangeSet = true;
		changeSet(false);
		ResourceManager::getInstance().getSound("changeSet").play();
		fight->fightRound(Attacks::SET, 1u);
	}
}

void GameStateGame::attackHead()
{
	if (interactionPermitted)
	{
		hideAttackGui();
		fight->fightRound(Attacks::HEAD, 1u);
	}
}

void GameStateGame::attackTorso()
{
	if (interactionPermitted)
	{
		hideAttackGui();
		fight->fightRound(Attacks::TORSO, 1u);
	}
}

void GameStateGame::attackArms()
{
	if (interactionPermitted)
	{
		hideAttackGui();
		fight->fightRound(Attacks::ARMS, 1u);
	}
}

void GameStateGame::attackLegs()
{
	if (interactionPermitted)
	{
		hideAttackGui();
		fight->fightRound(Attacks::LEGS, 1u);
	}
}

GameStateGame::~GameStateGame()
{
	delete map;
}