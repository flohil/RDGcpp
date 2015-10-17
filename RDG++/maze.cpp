#include "maze.hpp"
#include <iostream>
#include "random.hpp"
#include "easylogging++.hpp"

const std::set<ViewingDirections::Enum> MazeRoom::allDirs
{
	ViewingDirections::N,
	ViewingDirections::E,
	ViewingDirections::S,
	ViewingDirections::W
};

void Maze::generate()
{

	for (unsigned int x = 0; x < size.x; x++)
	{
		std::vector<std::shared_ptr<MazeRoom>> row;
		for (unsigned int y = 0; y < size.y; y++)
		{
			std::shared_ptr<MazeRoom> room(new MazeRoom(Point{ x, y }));
			row.push_back(room);

			if (x == start.x && y == start.y)
			{
				startSet.insert(room);
				notFinishedStartSet.insert(room);
			}
			else if (x == end.x && y == end.y)
			{
				endSet.insert(room);
				notFinishedEndSet.insert(room);
			}
			else
			{
				freeSet.insert(room);
			}
		}

		maze.push_back(row);
	}

	if (treasureRoom)
	{
		freeSet.erase(maze[treasure.x][treasure.y]);
	}

	while (!freeSet.empty())
	{
		if (!notFinishedStartSet.empty())
		{
			openRandomDoor(notFinishedStartSet, startSet, freeSet);
		}
		if (!notFinishedEndSet.empty())
		{
			openRandomDoor(notFinishedEndSet, endSet, freeSet);
		}
	}

	std::set<std::shared_ptr<MazeRoom>> borderStartSet;
	for (std::shared_ptr<MazeRoom> room : startSet)
	{
		for (ViewingDirections::Enum dir : MazeRoom::allDirs)
		{
			if (endSet.find(room->getAdjacentRoom(dir, *this)) != endSet.end())
			{
				borderStartSet.insert(room);
			}
		}
	}

	std::set<std::shared_ptr<MazeRoom>> borderEndSet;
	for (std::shared_ptr<MazeRoom> room : endSet)
	{
		for (ViewingDirections::Enum dir : MazeRoom::allDirs)
		{
			if (startSet.find(room->getAdjacentRoom(dir, *this)) != startSet.end())
			{
				borderEndSet.insert(room);
			}
		}
	}

	unsigned int paths = 0;
	while (paths < correctPaths)
	{
		if (borderStartSet.empty()) {
			break;
		}
		if (openRandomDoor(borderEndSet, startSet, borderStartSet, true))
		{
			paths++;
		}
	}

	if (treasureRoom)
	{
		std::shared_ptr<MazeRoom> room(maze[treasure.x][treasure.y]);
		room->openDoor(ViewingDirections::E, *this);
		room->openDoor(ViewingDirections::W, *this);
	}

	LOG(INFO) << "successfully generated maze with size " << size.x;
}

void Maze::print() const
{
	if (maze.empty())
	{
		return;
	}

	std::string resStr = "";

	for (unsigned int y = 0; y < maze[0].size(); y++) // column
	{
		for (unsigned int i = 0; i < 3; i++) // room height
		{
			for (unsigned int x = 0; x < maze.size(); x++) // row
			{
				if (i == 0)
				{
					resStr.append("#");

					if (maze[x][y]->isDoorOpen(ViewingDirections::N))
					{
						resStr.append("-");
					}
					else
					{
						resStr.append("#");
					}

					if (x == (maze.size() - 1))
					{
						resStr.append("#");
					}
				}
				else if (i == 1)
				{
					if (maze[x][y]->isDoorOpen(ViewingDirections::W))
					{
						resStr.append("|");
					}
					else
					{
						resStr.append("#");
					}

					resStr.append(" ");

					if (x == (maze.size() - 1))
					{
						if (maze[x][y]->isDoorOpen(ViewingDirections::E))
						{
							resStr.append("|");
						}
						else
						{
							resStr.append("#");
						}
					}
				}
				else if (y != (maze[0].size() - 1))
				{
					continue;
				}
				else
				{
					resStr.append("#");

					if (maze[x][y]->isDoorOpen(ViewingDirections::S))
					{
						resStr.append("-");
					}
					else
					{
						resStr.append("#");
					}

					if (x == (maze.size() - 1))
					{
						resStr.append("#");
					}
				}
			}

			if (i != 2)
			{
				resStr.append("\n");
			}
		}
	}

	LOG(DEBUG) << std::endl << resStr;
	//std::cout << resStr << std::endl;
}

std::shared_ptr<MazeRoom> Maze::getRoom(const Point pos) const
{
	if (pos.x < 0 || pos.x >(size.x - 1) || pos.y < 0 || pos.y >(size.y - 1))
	{
		return nullptr;
	}

	return maze[pos.x][pos.y];
};

Point Maze::getTreasurePos() const
{
	return treasure;
}

bool Maze::openRandomDoor(std::set<std::shared_ptr<MazeRoom>> &notFinished, std::set<std::shared_ptr<MazeRoom>> &additional, std::set<std::shared_ptr<MazeRoom>> &compare)
{
	return openRandomDoor(notFinished, additional, compare, false);
}

bool Maze::openRandomDoor(std::set<std::shared_ptr<MazeRoom>> &notFinished, std::set<std::shared_ptr<MazeRoom>> &additional, std::set<std::shared_ptr<MazeRoom>> &compare, bool ignoreFinished)
{
	// get all not finished Rooms and returns false if there are none
	std::vector<std::shared_ptr<MazeRoom>> rooms;

	if (notFinished.size() == 0)
	{
		return false;
	}

	for (std::shared_ptr<MazeRoom> r : notFinished)
	{
		rooms.push_back(r);
	}

	// get a random room
	std::shared_ptr<MazeRoom> room(*selectRandomly(rooms.begin(), rooms.end()));

	// get all closed doors of that random room and if there are none remove the room from the not finished set and return false
	std::vector<ViewingDirections::Enum> closedDoors = room->getClosedDoorsArray();

	if (closedDoors.size() == 0)
	{
		notFinished.erase(room);
		return false;
	}

	// get a random direction
	ViewingDirections::Enum dir = *selectRandomly(closedDoors.begin(), closedDoors.end());

	// open the door on that random direction
	std::shared_ptr<MazeRoom> connectedRoom = room->openDoor(dir, *this);

	// if the now connected room is a free one finish the opening process if not close the door
	if (compare.find(connectedRoom) != compare.end())
	{
		compare.erase(connectedRoom);
		notFinished.insert(connectedRoom);
		additional.insert(connectedRoom);
	}
	else
	{
		room->closeDoor(dir, *this);
		return false;
	}

	// finally check if the current room is already finished
	if (ignoreFinished)
	{
		return true;
	}

	unsigned int freeRooms = 0;

	// check for each possible direction if the adjacent room is in the freeSet
	for (ViewingDirections::Enum dir_ : MazeRoom::allDirs)
	{
		std::shared_ptr<MazeRoom> adjacentRoom = room->getAdjacentRoom(dir_, *this);

		if (adjacentRoom == nullptr)
		{
			continue;
		}
		if (compare.find(adjacentRoom) == compare.end())
		{
			continue;
		}

		freeRooms++;
	}

	// if the room has no free adjacent room it remove it from the notFinished set
	if (freeRooms == 0)
	{
		notFinished.erase(room);
	}

	return true;
}

std::shared_ptr<MazeRoom> MazeRoom::openDoor(ViewingDirections::Enum dir, Maze& maze)
{
	openDoors.insert(dir);
	std::shared_ptr<MazeRoom> room(nullptr);

	switch (dir)
	{
	case ViewingDirections::N:
		if ((room = maze.getRoom(Point{ pos.x, pos.y - 1 })) != nullptr)
		{
			room->_openDoor(ViewingDirections::S);
		}
		break;
	case ViewingDirections::E:
		if ((room = maze.getRoom(Point{ pos.x + 1, pos.y })) != nullptr)
		{
			room->_openDoor(ViewingDirections::W);
		}
		break;
	case ViewingDirections::S:
		if ((room = maze.getRoom(Point{ pos.x, pos.y + 1 })) != nullptr)
		{
			room->_openDoor(ViewingDirections::N);
		}
		break;
	case ViewingDirections::W:
		if ((room = maze.getRoom(Point{ pos.x - 1, pos.y })) != nullptr)
		{
			room->_openDoor(ViewingDirections::E);
		}
		break;
	}

	return room;
}

void MazeRoom::_openDoor(ViewingDirections::Enum dir) {
	openDoors.insert(dir);
}

std::shared_ptr<MazeRoom> MazeRoom::closeDoor(ViewingDirections::Enum dir, Maze& maze)
{
	openDoors.erase(dir);
	std::shared_ptr<MazeRoom> room(nullptr);

	switch (dir)
	{
	case ViewingDirections::N:
		if ((room = maze.getRoom(Point{ pos.x, pos.y - 1 })) != nullptr)
		{
			room->_closeDoor(ViewingDirections::S);
		}
		break;
	case ViewingDirections::E:
		if ((room = maze.getRoom(Point{ pos.x + 1, pos.y })) != nullptr)
		{
			room->_closeDoor(ViewingDirections::W);
		}
		break;
	case ViewingDirections::S:
		if ((room = maze.getRoom(Point{ pos.x, pos.y + 1 })) != nullptr)
		{
			room->_closeDoor(ViewingDirections::N);
		}
		break;
	case ViewingDirections::W:
		if ((room = maze.getRoom(Point{ pos.x - 1, pos.y })) != nullptr)
		{
			room->_closeDoor(ViewingDirections::E);
		}
		break;
	}

	return room;
}

void MazeRoom::_closeDoor(ViewingDirections::Enum dir) {
	openDoors.erase(dir);
}

bool MazeRoom::isDoorOpen(ViewingDirections::Enum dir) const {
	return (openDoors.find(dir) != openDoors.end()) ? true : false;
}

std::set<ViewingDirections::Enum> MazeRoom::getOpenDoors() const
{
	return openDoors;
}

std::set<ViewingDirections::Enum> MazeRoom::getClosedDoors() const
{
	std::set<ViewingDirections::Enum> closedDoors; // need to copy here?

	for (ViewingDirections::Enum dir : MazeRoom::allDirs)
	{
		if (openDoors.find(dir) == openDoors.end())
		{
			closedDoors.insert(dir); // closed doors are those which are not open doors
		}
	}

	return closedDoors;
}

std::vector<ViewingDirections::Enum> MazeRoom::getOpenDoorsArray() const
{
	std::vector<ViewingDirections::Enum> openDoorsArray;

	for (ViewingDirections::Enum dir : openDoors)
	{
		openDoorsArray.push_back(dir);
	}

	return openDoorsArray;
}

std::vector<ViewingDirections::Enum> MazeRoom::getClosedDoorsArray() const
{
	std::set<ViewingDirections::Enum> closedDoors = getClosedDoors();
	std::vector<ViewingDirections::Enum> closedDoorsArray;

	for (ViewingDirections::Enum dir : closedDoors)
	{
		closedDoorsArray.push_back(dir);
	}

	return closedDoorsArray;
}

std::shared_ptr<MazeRoom> MazeRoom::getAdjacentRoom(ViewingDirections::Enum dir, Maze& maze) const
{
	std::shared_ptr<MazeRoom> room(nullptr);

	switch (dir)
	{
	case ViewingDirections::N:
		room = maze.getRoom(Point{ pos.x, pos.y - 1 });
		break;
	case ViewingDirections::E:
		room = maze.getRoom(Point{ pos.x + 1, pos.y });
		break;
	case ViewingDirections::S:
		room = maze.getRoom(Point{ pos.x, pos.y + 1 });
		break;
	case ViewingDirections::W:
		room = maze.getRoom(Point{ pos.x - 1, pos.y });
		break;
	}

	return room;
}
