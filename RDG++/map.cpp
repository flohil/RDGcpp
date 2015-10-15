#include "map.hpp"
#include "easylogging++.hpp"

void Map::init()
{
	// null-initialize overlay and background
	for (unsigned int i = 0; i < width; i++)
	{
		std::vector<std::shared_ptr<RenderableObject>> overlayRow;
		std::vector<std::shared_ptr<RenderableObject>> backgroundRow;
		for (unsigned int j = 0; j < height; j++)
		{
			overlayRow.push_back(nullptr);
			backgroundRow.push_back(nullptr);
		}
		overlay.push_back(overlayRow);
		background.push_back(backgroundRow);
	}

	// null-initialize rooms
	for (unsigned int i = 0; i < settings->mazeSize; i++)
	{
		std::vector<std::shared_ptr<Room>> roomsRow;
		for (unsigned int j = 0; j < settings->mazeSize; j++)
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
	//fillWithRooms();

	LOG(INFO) << "successfully initialized map";
}

bool Map::isFieldPassable(Point fieldPos) const 
{
	if (overlay[fieldPos.x][fieldPos.y] != nullptr)
	{
		return false;
	}
	return true;
}

void Map::fillWithRooms() 
{
	for (unsigned int i = 0; i < settings->mazeSize; i++)
	{
		for (unsigned int j = 0; j < settings->mazeSize; j++)
		{
			for (unsigned int x = 0; x < settings->ROOM_WIDTH; x++)
			{
				for (unsigned int y = 0; y < settings->ROOM_HEIGHT; y++)
				{
					unsigned int posX = i * (settings->ROOM_WIDTH + 1) + x + 1;
					unsigned int posY = j * (settings->ROOM_HEIGHT + 1) + y + 1;

					background[posX][posY] = rooms[i][j]->background[x][y];
					overlay[posX][posY] = rooms[i][j]->overlay[x][y];
				}
			}
		}
	}
}

RoomTypes::Enum Map::detectRoomType(Point indizes) const
{
	// door positions -> set to true if there is a door a this position
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

	if (overlay[horDoorX][horDoorY1] == nullptr)
	{
		doorPos[0] = true;
		doorCount++;
	}
	if (overlay[verDoorX1][verDoorY] == nullptr)
	{
		doorPos[1] = true;
		doorCount++;
	}
	if (overlay[horDoorX][horDoorY2] == nullptr)
	{
		doorPos[2] = true;
		doorCount++;
	}
	if (overlay[verDoorX2][verDoorY] == nullptr)
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
	if ((indizes.x == settings->mazeSize / 2) && (indizes.y == settings->mazeSize / 2))
	{
		type = RoomTypes::TREASURECHAMBER;
	}

	return type;
}

void Map::loadRooms()
{
	for (unsigned int i = 0; i < settings->mazeSize; i++)
	{
		for (unsigned int j = 0; j < settings->mazeSize; j++)
		{
			//detect room types and load according room
			RoomTypes::Enum type = detectRoomType(Point{ i, j });
			
			std::shared_ptr<Room> room(game.getPrototypeStorage()->roomFactory->create(EnumMapper::mapRoomNames(type)));

			// null-initialize overlay and background
			for (unsigned int y = 0; y < settings->ROOM_WIDTH; y++)
			{
				std::vector<std::shared_ptr<RenderableObject>> overlayRow;
				std::vector<std::shared_ptr<RenderableObject>> backgroundRow;
				for (unsigned int x = 0; x < settings->ROOM_HEIGHT; x++)
				{
					overlayRow.push_back(nullptr);
					backgroundRow.push_back(nullptr);
				}
				room->overlay.push_back(overlayRow);
				room->background.push_back(backgroundRow);
			}

			/*fillGround(room, type);
			addMonster(room, type);
			addItems(room, type);*/

			rooms[i][j] = room;
		}
	}

	placeKey();
}

void Map::fillGround(std::shared_ptr<Room> room, RoomTypes::Enum type)
{
	// fill background according to room type
	for (unsigned int i = 0; i <= settings->ROOM_WIDTH - 1; i++) {
		for (unsigned int j = 0; j <= settings->ROOM_HEIGHT - 1; j++) {
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
		}
	}
}

void Map::addMonster(std::shared_ptr<Room> room, RoomTypes::Enum type)
{

}

void Map::addItems(std::shared_ptr<Room> room, RoomTypes::Enum type)
{

	//game.getPrototypeStorage()->roomFactory->getTemplate(EnumMapper::mapRoomNames(type));
	//Map<ItemClasses, Float> itemProbabilities = tempTemplate.getFind_probabilities();
	//int itemCount = tempTemplate.getItemCount();

	///* place items on random, free fields in the room */
	//for (int i = 0; i < itemCount; i++) {

	//	/* first find a free field, return null if no free field is found after 15 rounds */
	//	Point randPoint = Chances.randomFreeField(overlay);

	//	if (randPoint != null) { //no free field was found 

	//		/* get a random Item, according to the item levels allowed in this Room's definition*/
	//		Item item = Chances.randomItem(itemProbabilities, itemsBalance, balanceOffsets);

	//		if (item != null) { //no item shall be placed
	//			//use itemMultiplier
	//			room->overlay[randPoint.x][randPoint.y] = ItemFactory.createItem(item, tempTemplate.getItemMultiplier());
	//			map.increaseBalance("itemsBalance", null, item);
	//		}
	//	}
	//	else {
	//		break;
	//	}
	//}
}

// fills the map with walls and doors
void Map::fillWalls()
{
	// load walls and doors
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			unsigned int wallModX = i % (settings->ROOM_WIDTH + 1);
			unsigned int wallModY = j % (settings->ROOM_HEIGHT + 1);

			unsigned int roomX = i / (settings->ROOM_WIDTH + 1);

			// load walls and set them to not passable
			if (wallModX == 0 || wallModY == 0)
			{
				overlay[i][j].reset(new RenderableObject("darkGreyGround", ObjectType::TILE));
			}
		}
	}

	maze.reset(new Maze(settings->mazeSize));
	maze->generate();
	maze->print();

	// roomloop
	for (unsigned int i = 0; i < settings->mazeSize; i++) 
	{
		for (unsigned int j = 0; j < settings->mazeSize; j++) 
		{
			for (ViewingDirections::Enum dir : maze->getRoom(Point{ i, j })->getOpenDoors()){
				setDoors(Point{ i, j }, dir);
			}
		}
	}

	for (ViewingDirections::Enum dir : maze->getRoom(Point{ maze->getTreasurePos().x, maze->getTreasurePos().y })->getOpenDoors()){
		setDoors(Point{ maze->getTreasurePos().x, maze->getTreasurePos().y }, dir);
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
			angle = 0.0f;
			break;
		case ViewingDirections::E:
			doorx1 = (roomIndizes.x + 1) * (settings->ROOM_WIDTH + 1);
			doorx2 = (roomIndizes.x + 1) * (settings->ROOM_WIDTH + 1);
			doory1 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2);
			doory2 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2) + 1;
			angle = 90.0f;
			break;
		case ViewingDirections::S:
			doory1 = (roomIndizes.y + 1) * (settings->ROOM_HEIGHT + 1);
			doory2 = (roomIndizes.y + 1) * (settings->ROOM_HEIGHT + 1);
			doorx1 = roomIndizes.x * (settings->ROOM_WIDTH + 1) + (settings->ROOM_WIDTH / 2);
			doorx2 = roomIndizes.x * (settings->ROOM_WIDTH + 1) + (settings->ROOM_WIDTH / 2) + 1;
			angle = 180.0f;
			break;
		case ViewingDirections::W:
			doorx1 = roomIndizes.x * (settings->ROOM_WIDTH + 1);
			doorx2 = roomIndizes.x * (settings->ROOM_WIDTH + 1);
			doory1 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2);
			doory2 = roomIndizes.y * (settings->ROOM_HEIGHT + 1) + (settings->ROOM_HEIGHT / 2) + 1;
			angle = 270.0f;
			break;
	}
	
	// treasure chamber
	if (roomIndizes.x == maze->getTreasurePos().x && roomIndizes.y == maze->getTreasurePos().y) {
		background[doorx1][doory1].reset(new RenderableObject("doorGroundOne", ObjectType::TILE, angle));
		overlay[doorx1][doory1].reset(new RenderableObject("doorGroundOne", ObjectType::TILE, angle));
		background[doorx2][doory2].reset(new RenderableObject("doorGroundTwo", ObjectType::TILE, angle));
		overlay[doorx2][doory2].reset(new RenderableObject("doorGroundTwo", ObjectType::TILE, angle));
	}
	// normal door
	else {
		background[doorx1][doory1].reset(new RenderableObject("greyGround", ObjectType::TILE, angle));
		overlay[doorx1][doory1] = nullptr;
		background[doorx2][doory2].reset(new RenderableObject("greyGround", ObjectType::TILE, angle));
		overlay[doorx2][doory2] = nullptr;
	}
}

void Map::placeKey()
{
	// random rooms
	unsigned int randRoomX = (rand() % (unsigned int)settings->mazeSize);
	unsigned int randRoomY = 0;

	do
	{
		randRoomY = (rand() % (unsigned int)settings->mazeSize);
	} while (randRoomX == randRoomY);

	// random tiles in room
	unsigned int randTileX = (rand() % (unsigned int)settings->ROOM_WIDTH);
	unsigned int randTileY = 0;

	do
	{
		randRoomY = (rand() % (unsigned int)settings->ROOM_HEIGHT);
	} while (randRoomX == randRoomY);

	std::cout << "placing key at room( " << randRoomX << ", " << randRoomY << ") at tile (" << randTileX << ", " << randTileY << ")" << std::endl;

	std::cout << "rooms size: " << rooms.size() << " x " << rooms[0].size() << std::endl;
	std::cout << "room background size: " << rooms[randRoomX][randRoomY]->background.size() << " x " << rooms[randRoomX][randRoomY]->background[0].size() << std::endl;
	std::cout << "room overlay size: " << rooms[randRoomX][randRoomY]->overlay.size() << " x " << rooms[randRoomX][randRoomY]->overlay[0].size() << std::endl;

	std::shared_ptr<Room> test = rooms[randRoomX][randRoomY];
	std::shared_ptr<RenderableObject> bla = test->overlay[randTileX][randTileY];

	if (bla == nullptr)
	{
		std::cout << "yes its a nullptr" << std::endl;
	}

	//rooms[randRoomX][randRoomY]->overlay[randTileX][randTileY].reset(new RenderableObject("key", ObjectType::KEY));
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