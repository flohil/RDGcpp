#include "maze.hpp"

const std::set<ViewingDirections::Enum> MazeRoom::allDirs 
{
	ViewingDirections::N,
	ViewingDirections::E,
	ViewingDirections::S,
	ViewingDirections::W
};

//std::shared_ptr<MazeRoom> MazeRoom::openDoor(ViewingDirections::Enum dir, Maze& maze)
//{
//	openDoors.insert(dir);
//	std::shared_ptr<MazeRoom> room(nullptr);
//
//	switch (dir)
//	{
//		case ViewingDirections::N:
//			if ((room = maze.getRoom(Point{ pos.x, pos.y - 1 })) != nullptr)
//			{
//				room->_openDoor(ViewingDirections::S);
//			}
//			break;
//		case ViewingDirections::E:
//			if ((room = maze.getRoom(Point{ pos.x + 1, pos.y })) != nullptr)
//			{
//				room->_openDoor(ViewingDirections::W);
//			}
//			break;
//		case ViewingDirections::S:
//			if ((room = maze.getRoom(Point{ pos.x, pos.y + 1 })) != nullptr)
//			{
//				room->_openDoor(ViewingDirections::N);
//			}
//			break;
//		case ViewingDirections::W:
//			if ((room = maze.getRoom(Point{ pos.x - 1, pos.y })) != nullptr)
//			{
//				room->_openDoor(ViewingDirections::E);
//			}
//			break;
//	}
//	
//	return room;
//}