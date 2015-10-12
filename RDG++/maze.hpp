#ifndef MAZE
#define MAZE

#include "enums.hpp"
#include <set>

class MazeRoom
{
public:

	friend class Maze;

	static const std::set<ViewingDirections> allDirs;

	MazeRoom(const unsigned int posX_, const unsigned int posY_) : posX(posX_), posY(posY_) {}


private:

	std::set<ViewingDirections> openDoors;
	const unsigned int posX;
	const unsigned int posY;


};

class Maze
{
public:

private:

	const unsigned int sizeX;
	const unsigned int sizeY;
	const unsigned int startX;
	const unsigned int startY;
	const unsigned int endX;
	const unsigned int endY;
	const bool treasureRoom;
	const unsigned int correctPaths;

	unsigned int treasureX;
	unsigned int treasureY;
	//set<MazeRoom> 
};

#endif // MAZE
