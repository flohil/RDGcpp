#include "map.hpp"


void Map::init()
{
	// null-initialize overlay
	for (unsigned int i = 0; i < width; i++)
	{
		std::vector<std::shared_ptr<RenderableObject>> row;
		for (unsigned int j = 0; j < height; j++)
		{
			row.push_back(nullptr);
		}
		overlay.push_back(row);
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

	/*fillWalls();
	loadRooms();
	fillWithRooms();*/
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

					background[posX][posY] = rooms[i][j]->getBackround()[x][y];
					overlay[posX][posY] = rooms[i][j]->getOverlay()[x][y];
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
			
			rooms[i][j] = game.getPrototypeStorage()->roomFactory->create(EnumMapper::mapRoomNames(type));
		}
	}

	placeKey();
}

// fills the map with walls and doors
void Map::fillWalls()
{
	// load walls and doors
	for (unsigned int i = 0; i <= width; i++)
	{
		for (unsigned int j = 0; j <= height; j++)
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

	Maze maze(settings->mazeSize);
	maze.generate();

	// roomloop
	for (unsigned int i = 0; i < settings->mazeSize; i++) 
	{
		for (unsigned int j = 0; j < settings->mazeSize; j++) 
		{
			for (ViewingDirections::Enum dir : maze.getRoom(Point{ i, j })->getOpenDoors()){
				setDoors(Point{ i, j }, dir);
			}
		}
	}

	for (ViewingDirections::Enum dir : maze.getRoom(Point{ maze.getTreasurePos().x, maze.getTreasurePos().y })->getOpenDoors()){
		setDoors(Point{ maze.getTreasurePos().x, maze.getTreasurePos().y }, dir);
	}
}

void Map::setDoors(Point roomIndizes, ViewingDirections::Enum dir)
{

}

void Map::placeKey()
{

}

void increaseBalance(std::string balanceMap, std::string name, ItemType itemType)
{

}