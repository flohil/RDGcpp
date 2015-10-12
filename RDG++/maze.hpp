#ifndef MAZE
#define MAZE

#include "enums.hpp"
#include <set>
#include <vector>
#include <memory>

struct Point {
	unsigned int x;
	unsigned int y;
};

class MazeRoom
{
public:

	friend class Maze;

	static const std::set<ViewingDirections::Enum> allDirs;

	MazeRoom(const Point pos_) : pos(pos_) {};


private:

	std::set<ViewingDirections::Enum> openDoors;
	const Point pos;

	std::shared_ptr<MazeRoom> openDoor(ViewingDirections::Enum dir, Maze& maze);
	void _openDoor(ViewingDirections::Enum dir);
	std::shared_ptr<MazeRoom> closeDoor(ViewingDirections::Enum dir, Maze& maze);
	void _closeDoor(ViewingDirections::Enum dir);
	std::shared_ptr<MazeRoom> isDoorOpen(ViewingDirections::Enum dir) const;
	std::set<ViewingDirections::Enum> getOpenDoors() const;
	std::set<ViewingDirections::Enum> getClosedDoors() const;
	ViewingDirections::Enum* getOpenDoorsArray() const;
	ViewingDirections::Enum* getClosedDoorsArray() const;
	std::shared_ptr<MazeRoom> getAdjacentRoom(ViewingDirections::Enum dir) const;
};

class Maze
{
public:

	Maze(const unsigned int size_) : Maze(Point{ size_, size_ }) {};
	Maze(const Point size_) : Maze(size_, Point{ 0u, 0u }, Point{ size_.x - 1, size_.y - 1 }, true, 1u, Point{ size_.x / 2, size_.y / 2 }) {};
	Maze(const Point size_, bool treasureRoom_, const unsigned int correctPaths_) : Maze(size_, Point{ 0u, 0u }, Point{ size_.x - 1, size_.y - 1 }, treasureRoom_, correctPaths_, Point{ size_.x / 2, size_.y / 2 }) {};
	Maze(const Point size_, const Point start_, const Point end_, bool treasureRoom_, const unsigned int correctPaths_, const Point treasure_) :
		size(size_), start(start_), end(end_), treasureRoom(treasureRoom_), correctPaths(correctPaths_), treasure(treasure_) {};

	void generate();
	void print() const;
	std::shared_ptr<MazeRoom> getRoom(const Point pos);

private:

	const Point size;
	const Point start;
	const Point end;
	const bool treasureRoom;
	const unsigned int correctPaths;
	const Point treasure;

	std::set<MazeRoom> startSet;
	std::set<MazeRoom> notFinishedStartSet;
	std::set<MazeRoom> endSet;
	std::set<MazeRoom> notFinishedEndSet;
	std::set<MazeRoom> freeSet;

	std::vector<std::vector<MazeRoom>> maze;
};

#endif // MAZE
