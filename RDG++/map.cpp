#include "map.hpp"
#include "easylogging++.hpp"
#include "chances.hpp"

void Map::init(std::shared_ptr<Player> player_)
{
	player = player_;

	// construct maze
	maze.reset(new Maze(settings->mazeSize));
	maze->generate();
	maze->print();

	// null-initialize overlay and background
	for (unsigned int i = 0; i < height; i++)
	{
		std::vector<std::shared_ptr<RenderableObject>> overlayRow;
		std::vector<std::shared_ptr<RenderableObject>> backgroundRow;
		for (unsigned int j = 0; j < width; j++)
		{
			overlayRow.push_back(nullptr);
			backgroundRow.push_back(nullptr);
		}
		overlay.push_back(overlayRow);
		background.push_back(backgroundRow);
	}

	// null-initialize rooms
	for (unsigned int i = 0; i < maze->getSize().y; i++) //rows
	{
		std::vector<std::shared_ptr<Room>> roomsRow;
		for (unsigned int j = 0; j < maze->getSize().x; j++) //cols
		{
			roomsRow.push_back(nullptr);
		}
		rooms.push_back(roomsRow);
	}

	monsterBalanceOffsets.insert(std::pair<std::string, const unsigned int>("easy", easyMonsterOffset));
	monsterBalanceOffsets.insert(std::pair<std::string, const unsigned int>("normal", normalMonsterOffset));
	monsterBalanceOffsets.insert(std::pair<std::string, const unsigned int>("hard", hardMonsterOffset));

	itemBalanceOffsets.insert(std::pair<std::string, const unsigned int>("weak", weakItemOffset));
	itemBalanceOffsets.insert(std::pair<std::string, const unsigned int>("medium", mediumItemOffset));
	itemBalanceOffsets.insert(std::pair<std::string, const unsigned int>("strong", strongItemOffset));

	std::map<DifficultyLevel::Enum, std::list<std::string>>& monstersLeveled = game.getPrototypeStorage()->getMonstersLeveled();

	for (std::string monster : monstersLeveled.at(DifficultyLevel::EASY))
	{
		easyMap.insert(std::pair<std::string, unsigned int>(monster, 0));
	}
	for (std::string monster : monstersLeveled.at(DifficultyLevel::NORMAL))
	{
		normalMap.insert(std::pair<std::string, unsigned int>(monster, 0));
	}
	for (std::string monster : monstersLeveled.at(DifficultyLevel::HARD))
	{
		hardMap.insert(std::pair<std::string, unsigned int>(monster, 0));
	}

	std::map<Classes::Enum, std::list<std::pair<std::string, ItemType::Enum>>>& itemsClassList = game.getPrototypeStorage()->getItemsClassList();

	for (std::pair<std::string, ItemType::Enum> itemPair : itemsClassList.at(Classes::WEAK))
	{
		weakMap.insert(std::pair<std::string, ItemBalance>(itemPair.first, ItemBalance{itemPair.second, 0}));
	}
	for (std::pair<std::string, ItemType::Enum> itemPair : itemsClassList.at(Classes::MEDIUM))
	{
		mediumMap.insert(std::pair<std::string, ItemBalance>(itemPair.first, ItemBalance{ itemPair.second, 0 }));
	}
	for (std::pair<std::string, ItemType::Enum> itemPair : itemsClassList.at(Classes::STRONG))
	{
		strongMap.insert(std::pair<std::string, ItemBalance>(itemPair.first, ItemBalance{ itemPair.second, 0 }));
	}

	typedef std::pair<DifficultyLevel::Enum, std::map<std::string, unsigned int>> mbPair;
	monsterBalance.insert(mbPair(DifficultyLevel::EASY, easyMap));
	monsterBalance.insert(mbPair(DifficultyLevel::NORMAL, normalMap));
	monsterBalance.insert(mbPair(DifficultyLevel::HARD, hardMap));

	typedef std::pair<Classes::Enum, std::map<std::string, ItemBalance>> ibPair;
	itemsBalance.insert(ibPair(Classes::WEAK, weakMap));
	itemsBalance.insert(ibPair(Classes::MEDIUM, mediumMap));
	itemsBalance.insert(ibPair(Classes::STRONG, strongMap));

	fillWalls();
	loadRooms();
	fillWithRooms();

	/*placeInOverlay(Point{ 1, 1 }, std::shared_ptr<RenderableObject>(game.getPrototypeStorage()->weaponFactory->create("Bow")));
	placeInOverlay(Point{ 2, 1 }, std::shared_ptr<RenderableObject>(game.getPrototypeStorage()->weaponFactory->create("Axt")));*/

	LOG(INFO) << "successfully initialized map";
}

// fills the map with walls and doors
void Map::fillWalls()
{
	// load walls and doors
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			unsigned int wallModY = i % (settings->ROOM_HEIGHT + 1);
			unsigned int wallModX = j % (settings->ROOM_WIDTH + 1);
			
			// load walls and set them to not passable
			if (wallModX == 0 || wallModY == 0)
			{
				placeInOverlay(Point{ j, i }, std::shared_ptr<RenderableObject>(new RenderableObject("darkGreyGround", ObjectType::TILE)));
			}
		}
	}

	// roomloop
	for (unsigned int i = 0; i < maze->getSize().y; i++) //height
	{
		for (unsigned int j = 0; j < maze->getSize().x; j++) //width
		{
			for (ViewingDirections::Enum dir : maze->getRoom(Point{ j, i })->getOpenDoors())
			{
				setDoors(Point{ j, i }, dir);
			}
		}
	}

	for (ViewingDirections::Enum dir : maze->getRoom(Point{ maze->getTreasurePos().x, maze->getTreasurePos().y })->getOpenDoors())
	{
		setDoors(Point{ maze->getTreasurePos().x, maze->getTreasurePos().y }, dir);
	}
}

void Map::loadRooms()
{
	for (unsigned int i = 0; i < maze->getSize().y; i++) //rows
	{
		for (unsigned int j = 0; j < maze->getSize().x; j++) //cols
		{
			// detect room types and load according room
			RoomTypes::Enum type = detectRoomType(Point{ j, i });

			std::shared_ptr<Room> room(game.getPrototypeStorage()->roomFactory->create(EnumMapper::mapRoomNames(type)));

			room->initialize(settings->ROOM_WIDTH, settings->ROOM_HEIGHT);

			fillGround(room, type);
			addMonster(room, type);
			addItems(room, type);

			rooms[i][j] = room;
		}
	}

	placeKey();
}

void Map::fillWithRooms() 
{

	for (unsigned int i = 0; i < settings->mazeSize; i++)
	{
		for (unsigned int j = 0; j < settings->mazeSize; j++)
		{
			for (unsigned int y = 0; y < settings->ROOM_HEIGHT; y++)
			{
				for (unsigned int x = 0; x < settings->ROOM_WIDTH; x++)
				{
					unsigned int posY = i * (settings->ROOM_HEIGHT + 1) + y + 1; //row
					unsigned int posX = j * (settings->ROOM_WIDTH + 1) + x + 1; //column

					placeInBackground(Point{ posX, posY }, std::shared_ptr<RenderableObject>(rooms[i][j]->background[y][x]));
					placeInOverlay(Point{ posX, posY }, std::shared_ptr<RenderableObject>(rooms[i][j]->overlay[y][x]));
				}
			}
		}
	}
}

void Map::setDoors(Point roomIndizes, ViewingDirections::Enum dir)
{

	unsigned int doorx1 = 0;
	unsigned int doorx2 = 0;
	unsigned int doory1 = 0;
	unsigned int doory2 = 0;
	float angle = 0;

	switch (dir) {
	case ViewingDirections::N:
		doory1 = roomIndizes.y * (settings->ROOM_HEIGHT + 1);
		doory2 = roomIndizes.y * (settings->ROOM_HEIGHT + 1);
		doorx1 = roomIndizes.x * (settings->ROOM_WIDTH + 1) + (settings->ROOM_WIDTH / 2);
		doorx2 = roomIndizes.x * (settings->ROOM_WIDTH + 1) + (settings->ROOM_WIDTH / 2) + 1;
		if (roomIndizes.x == maze->getTreasurePos().x && roomIndizes.y == maze->getTreasurePos().y)
		{
			angle = 270.0f;
		}
		else {
			angle = 0.0f;
		}
		break;
	case ViewingDirections::E:
		doorx1 = (roomIndizes.x + 1) * (settings->ROOM_WIDTH + 1);
		doorx2 = (roomIndizes.x + 1) * (settings->ROOM_WIDTH + 1);
		doory1 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2);
		doory2 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2) + 1;
		if (roomIndizes.x == maze->getTreasurePos().x && roomIndizes.y == maze->getTreasurePos().y)
		{
			angle = 0.0f;
		}
		else {
			angle = 90.0f;
		}
		break;
	case ViewingDirections::S:
		doory1 = (roomIndizes.y + 1) * (settings->ROOM_HEIGHT + 1);
		doory2 = (roomIndizes.y + 1) * (settings->ROOM_HEIGHT + 1);
		doorx1 = roomIndizes.x * (settings->ROOM_WIDTH + 1) + (settings->ROOM_WIDTH / 2);
		doorx2 = roomIndizes.x * (settings->ROOM_WIDTH + 1) + (settings->ROOM_WIDTH / 2) + 1;
		if (roomIndizes.x == maze->getTreasurePos().x && roomIndizes.y == maze->getTreasurePos().y)
		{
			angle = 270.0f;
		}
		else {
			angle = 180.0f;
		}
		break;
	case ViewingDirections::W:
		doorx1 = roomIndizes.x * (settings->ROOM_WIDTH + 1);
		doorx2 = roomIndizes.x * (settings->ROOM_WIDTH + 1);
		doory1 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2);
		doory2 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2) + 1;
		if (roomIndizes.x == maze->getTreasurePos().x && roomIndizes.y == maze->getTreasurePos().y)
		{
			angle = 0.0f;
		}
		else {
			angle = 270.0f;
		}
		break;
	}

	// treasure chamber
	if (roomIndizes.x == maze->getTreasurePos().x && roomIndizes.y == maze->getTreasurePos().y)
	{
		treasureDoorOneAngle = angle + 180.f;
		treasureDoorTwoAngle = angle;

		placeInBackground(Point{ doorx1, doory1 }, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundOne", ObjectType::TILE, treasureDoorOneAngle)));
		placeInOverlay(Point{ doorx1, doory1 }, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundOne", ObjectType::TILE, treasureDoorOneAngle)));
		placeInBackground(Point{ doorx2, doory2 }, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundTwo", ObjectType::TILE, treasureDoorTwoAngle)));
		placeInOverlay(Point{ doorx2, doory2 }, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundTwo", ObjectType::TILE, treasureDoorTwoAngle)));

		treasureDoorOne = Point{ doorx1, doory1 };
		treasureDoorTwo = Point{ doorx2, doory2 };

	}
	// normal door
	else
	{
		placeInBackground(Point{ doorx1, doory1 }, std::shared_ptr<RenderableObject>(new RenderableObject("greyGround", ObjectType::TILE, angle)));
		placeInBackground(Point{ doorx2, doory2 }, std::shared_ptr<RenderableObject>(new RenderableObject("greyGround", ObjectType::TILE, angle)));
		placeInOverlay(Point{ doorx1, doory1 }, nullptr);
		placeInOverlay(Point{ doorx2, doory2 }, nullptr);
	}
}

void Map::fillGround(std::shared_ptr<Room> room, RoomTypes::Enum type)
{
	// fill background according to room type
	for (unsigned int i = 0; i < room->background.size(); i++) {
		for (unsigned int j = 0; j < room->background[0].size(); j++) {
			switch (type) {
			case RoomTypes::DEADEND:
				room->background[i][j].reset(new RenderableObject("yellowOneGround", ObjectType::TILE));
				break;
			case RoomTypes::HALLWAY:
				room->background[i][j].reset(new RenderableObject("greenGround", ObjectType::TILE));
				break;
			case RoomTypes::TURN:
				room->background[i][j].reset(new RenderableObject("greenGround", ObjectType::TILE));
				break;
			case RoomTypes::TJUNCTION:
				room->background[i][j].reset(new RenderableObject("yellowTwoGround", ObjectType::TILE));
				break;
			case RoomTypes::JUNCTION:
				room->background[i][j].reset(new RenderableObject("yellowOneGround", ObjectType::TILE));
				break;
			case RoomTypes::TREASURECHAMBER:
				room->background[i][j].reset(new RenderableObject("brownGround", ObjectType::TILE));
				break;
			}
			room->background[i][j]->setSize(settings->tileSize, settings->tileSize);
		}
	}
}

RoomTypes::Enum Map::detectRoomType(Point indizes) const
{
	// door positions -> set to true if there is a door at this position
	bool doorPos[4] { false, false, false, false }; // N, E, S, W

	const unsigned int roomWidth = settings->ROOM_WIDTH;
	const unsigned int roomHeight = settings->ROOM_HEIGHT;

	unsigned int horDoorX = (indizes.x + 1) * (roomWidth + 1) - (roomWidth / 2) - 1;
	unsigned int horDoorY1 = indizes.y * (roomHeight + 1);
	unsigned int horDoorY2 = (indizes.y + 1) * (roomHeight + 1);
	unsigned int verDoorX1 = indizes.x * (roomWidth + 1);
	unsigned int verDoorX2 = (indizes.x + 1) * (roomWidth + 1);
	unsigned int verDoorY = (indizes.y + 1) * (roomHeight + 1) - roomHeight / 2 - 1;

	unsigned int doorCount = 0;

	RoomTypes::Enum type = RoomTypes::UNKNOWN;

	if (overlay[horDoorY1][horDoorX] == nullptr)
	{
		doorPos[0] = true;
		doorCount++;
	}
	if (overlay[verDoorY][verDoorX1] == nullptr)
	{
		doorPos[1] = true;
		doorCount++;
	}
	if (overlay[horDoorY2][horDoorX] == nullptr)
	{
		doorPos[2] = true;
		doorCount++;
	}
	if (overlay[verDoorY][verDoorX2] == nullptr)
	{
		doorPos[3] = true;
		doorCount++;
	}

	// detect room types based on the number of doors - for 2 doors also check door positions
	switch (doorCount)
	{
	case 1:
		type = RoomTypes::DEADEND;
		break;
	case 2:
		for (unsigned int z = 0; z < 4; z++)
		{
			unsigned int posTurn = (z + 1) % 4;
			unsigned int posHallway = (z + 2) % 4;

			if (doorPos[z] && doorPos[posTurn])
			{
				type = RoomTypes::TURN;
				break;
			}
			if (doorPos[z] && doorPos[posHallway])
			{
				type = RoomTypes::HALLWAY;
				break;
			}
		}
		break;
	case 3:
		type = RoomTypes::TJUNCTION;
		break;
	case 4:
		type = RoomTypes::JUNCTION;
		break;
	}

	// check for treasure chamber -> middle of the map
	if ((indizes.x == maze->getSize().x / 2) && (indizes.y == maze->getSize().y / 2))
	{
		type = RoomTypes::TREASURECHAMBER;
	}

	return type;
}

void Map::addMonster(std::shared_ptr<Room> room, RoomTypes::Enum type)
{
	std::shared_ptr<RoomTemplate> roomTemplate = std::dynamic_pointer_cast<RoomTemplate>(game.getPrototypeStorage()->roomFactory->getTemplate(EnumMapper::mapRoomNames(type)));

	// place monsters on random, free fields in the room
	for (unsigned int i = 0; i < roomTemplate->getMonsterCount(); i++)
	{

		// first find a free field, return null if no free field is found after 15 rounds
		FoundPoint randPoint = Chances::randomFreeTile(room->overlay, settings->MAX_TRIES);

		if (randPoint.found != false) //a free field was found 
		{
			// get a random Monster, according to the monster levels allowed in this Room's definition
			std::string monsterName = Chances::randomMonster(roomTemplate->getMonsterProbabilities(), monsterBalance, monsterBalanceOffsets, game.getPrototypeStorage()->monsterFactory->getMonstersLeveled());

			if (monsterName != "") //no monster shall be placed
			{
				room->overlay[randPoint.point.y][randPoint.point.x] = game.getPrototypeStorage()->monsterFactory->create(monsterName);
				room->overlay[randPoint.point.y][randPoint.point.x]->setSize(settings->tileSize, settings->tileSize);
				increaseMonsterBalance(monsterName);
			}
		}
		else {
			break;
		}
	}
}

void Map::addItems(std::shared_ptr<Room> room, RoomTypes::Enum type)
{
	std::shared_ptr<RoomTemplate> roomTemplate = std::dynamic_pointer_cast<RoomTemplate>(game.getPrototypeStorage()->roomFactory->getTemplate(EnumMapper::mapRoomNames(type)));

	// place items on random, free fields in the room
	for (unsigned int i = 0; i < roomTemplate->getItemCount(); i++)
	{
		// first find a free field, return null if no free field is found after 15 rounds
		FoundPoint randPoint = Chances::randomFreeTile(room->overlay, settings->MAX_TRIES);

		if (randPoint.found != false) //a free field was found 
		{

			// get a random Monster, according to the monster levels allowed in this Room's definition
			ItemTypeName item = Chances::randomItem(roomTemplate->getItemProbabilities(), itemsBalance, itemBalanceOffsets, game.getPrototypeStorage()->getItemsClassList());

			if (item.itemName != "") // no item shall be placed
			{
				switch (item.itemType)
				{
					case ItemType::ARMAMENT:
						room->overlay[randPoint.point.y][randPoint.point.x] = game.getPrototypeStorage()->armamentFactory->create(item.itemName);
						room->overlay[randPoint.point.y][randPoint.point.x]->setSize(settings->tileSize, settings->tileSize);
						break;
					case ItemType::POTION:
						room->overlay[randPoint.point.y][randPoint.point.x] = game.getPrototypeStorage()->potionFactory->create(item.itemName);
						room->overlay[randPoint.point.y][randPoint.point.x]->setSize(settings->tileSize, settings->tileSize);
						break;
					case ItemType::WEAPON:
						room->overlay[randPoint.point.y][randPoint.point.x] = game.getPrototypeStorage()->weaponFactory->create(item.itemName);
						room->overlay[randPoint.point.y][randPoint.point.x]->setSize(settings->tileSize, settings->tileSize);
						break;
					default:
						break;
				}
				increaseItemBalance(item.itemName);
			}
		}
		else {
			break;
		}
	}
}

void Map::placeKey()
{
	bool placedKey = false;

	Point randRoom;
	FoundPoint randTile;

	do
	{
		randRoom = Chances::randomRoom(settings->mazeSize, settings->mazeSize);

		randTile = Chances::randomFreeTile(rooms[randRoom.y][randRoom.x]->overlay, settings->MAX_TRIES);

		if (randTile.found)
		{
			placedKey = true;
		}

	} while (!placedKey);

	rooms[randRoom.y][randRoom.x]->overlay[randTile.point.y][randTile.point.x].reset(new RenderableObject("key", ObjectType::KEY));
	rooms[randRoom.y][randRoom.x]->overlay[randTile.point.y][randTile.point.x]->setSize(settings->tileSize, settings->tileSize);
}

// Increase the balance counter for added monster
void Map::increaseMonsterBalance(std::string name)
{
	typedef std::map<DifficultyLevel::Enum, std::map<std::string, unsigned int>> itType;

	for (itType::iterator it = monsterBalance.begin(); it != monsterBalance.end(); ++it)
	{
		if (it->second.find(name) != it->second.end())
		{
			it->second[name]++;
			return;
		}
	}
}

// Increase the balance counter for an added item
void Map::increaseItemBalance(std::string name)
{
	typedef std::map<Classes::Enum, std::map<std::string, ItemBalance>> itType;

	for (itType::iterator it = itemsBalance.begin(); it != itemsBalance.end(); ++it)
	{
		if (it->second.find(name) != it->second.end())
		{
			it->second[name].balanceCount++;
			return;
		}
	}
}

bool Map::isFieldPassable(Point fieldPos) const
{
	if (overlay[fieldPos.y][fieldPos.x] != nullptr)
	{
		return false;
	}
	return true;
}

void Map::draw(sf::RenderWindow& window, float deltaTime)
{
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			if (background[y][x] != nullptr)
			{
				background[y][x]->draw(window, deltaTime);
			}
			
			if (overlay[y][x] != nullptr)
			{
				overlay[y][x]->draw(window, deltaTime);
			}
		}
	}
	return;
}

Point Map::initPlayerPosition()
{
	Point randPoint = Chances::randomFreeTile(rooms[0][0]->overlay, settings->MAX_TRIES).point;
	initialPlayerPosition = Point{ randPoint.x + 1, randPoint.y + 1 };

	return initialPlayerPosition; // consider wall 
}

void Map::update(const float deltaTime)
{
	/*overlay[player->getPrevPosition().x][player->getPrevPosition().y] = nullptr;
	overlay[player->getPosition().x][player->getPosition().y] = player;*/
}

void Map::openTreasureChamber()
{
	if (!treasureDoorOpened)
	{
		overlay[treasureDoorOne.y][treasureDoorOne.x] = nullptr;
		overlay[treasureDoorTwo.y][treasureDoorTwo.x] = nullptr;

		treasureDoorOpened = true;
	}
}

void Map::closeTreasureChamber()
{
	treasureDoorOpened = false;
	placeInBackground(treasureDoorOne, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundOne", ObjectType::TILE, treasureDoorOneAngle)));
	placeInOverlay(treasureDoorOne, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundOne", ObjectType::TILE, treasureDoorOneAngle)));
	placeInBackground(treasureDoorTwo, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundTwo", ObjectType::TILE, treasureDoorTwoAngle)));
	placeInOverlay(treasureDoorTwo, std::shared_ptr<RenderableObject>(new RenderableObject("doorGroundTwo", ObjectType::TILE, treasureDoorTwoAngle)));
}

std::shared_ptr<RenderableObject> Map::placeInOverlay(Point pos, std::shared_ptr<RenderableObject> obj)
{
	std::shared_ptr<RenderableObject> oldObj = overlay[pos.y][pos.x];

	if (obj != nullptr)
	{
		sf::Vector2f pixelPos;
		pixelPos.x = static_cast<float>(pos.x * settings->tileSize);
		pixelPos.y = static_cast<float>(pos.y * settings->tileSize);
		obj->setSize(settings->tileSize, settings->tileSize);
		obj->setPosition(sf::Vector2f(pixelPos.x, pixelPos.y));
	}

	overlay[pos.y][pos.x] = obj;

	return oldObj;
}

std::shared_ptr<RenderableObject> Map::placeInBackground(Point pos, std::shared_ptr<RenderableObject> obj)
{
	std::shared_ptr<RenderableObject> oldObj = overlay[pos.y][pos.x];

	if (obj != nullptr)
	{
		sf::Vector2f pixelPos;
		pixelPos.x = static_cast<float>(pos.x * settings->tileSize);
		pixelPos.y = static_cast<float>(pos.y * settings->tileSize);
		obj->setSize(settings->tileSize, settings->tileSize);
		obj->setPosition(sf::Vector2f(pixelPos.x, pixelPos.y));
	}

	background[pos.y][pos.x] = obj;

	return oldObj;
}

std::shared_ptr<RenderableObject> Map::handleItemAtPixels(sf::Vector2i pixelPos, bool remove, std::shared_ptr<RenderableObject> obj)
{
	sf::Vector2u pos;

	if (pixelPos.x < 0 || pixelPos.y < 0)
	{
		return nullptr;
	}

	pos.x = static_cast<unsigned int>(pixelPos.x / settings->tileSize);
	pos.y = static_cast<unsigned int>(pixelPos.y / settings->tileSize);

	if (pos.y > overlay.size() || pos.x > overlay[0].size())
	{
		return nullptr;
	}
	else
	{
		std::shared_ptr<RenderableObject> retObj = overlay[pos.y][pos.x];

		if (remove)
		{
			overlay[pos.y][pos.x] = nullptr;
		}

		if (obj != nullptr)
		{
			overlay[pos.y][pos.x] = nullptr;
		}

		return retObj;
	}
}