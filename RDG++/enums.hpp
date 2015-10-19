#ifndef ENUMS_INCLUDE
#define ENUMS_INCLUDE

#include <string>

struct ObjectType
{
	enum Enum { ITEM, CREATURE, ATTACK, ROOM, TILE, WALL, KEY, UNKNWON };
};

struct ItemType
{
	enum Enum { WEAPON, ARMAMENT, POTION, UNKNOWN };
};

struct CreatureType
{
	enum Enum { MONSTER, PLAYER, UNKNOWN };
};

struct ViewingDirections
{
	enum Enum { N, E, S, W, UNKNOWN };
};

struct DifficultyLevel
{
	enum Enum { EASY, NORMAL, HARD, UNKNOWN };
};

struct Classes
{
	enum Enum { NONE, WEAK, MEDIUM, STRONG, UNKNOWN };
};

struct Target
{
	enum Enum { SELF, OPPONENT, UNKNOWN };
};

struct Attribute
{
	enum Enum { HP, SPEED, ACCURACY, STRENGTH, UNKNOWN };
};

struct Mode
{
	enum Enum { CURE, TEMPORARY_DECREASE, INCREMENTAL_DECREASE, TEMPORARY_INCREASE, INCREMENTAL_INCREASE, UNKNOWN };
};

struct RoomTypes
{
	enum Enum { DEADEND, HALLWAY, TURN, TJUNCTION, JUNCTION, TREASURECHAMBER, UNKNOWN };
};

struct WeaponType
{
	enum Enum { SINGLE, DOUBLE, UNKNOWN };
};

struct Attacks
{
	enum Enum { TORSO, HEAD, ARMS, LEGS, PARRY, SET, POTION, UNKNOWN };
};


struct Point {
	unsigned int x;
	unsigned int y;

	Point getDirPoint(ViewingDirections::Enum dir)
	{
		switch (dir)
		{
			case ViewingDirections::N:
				return Point{ x, y - 1 };
			case ViewingDirections::E:
				return Point{ x + 1, y };
			case ViewingDirections::S:
				return Point{ x, y + 1 };
			case ViewingDirections::W:
				return Point{ x - 1, y };
		}
	}
};

struct ItemBalance
{
	ItemType::Enum itemType;
	unsigned int balanceCount;
};

struct ItemTypeName
{
	std::string itemName;
	ItemType::Enum itemType;
};

struct FoundPoint
{
	Point point;
	bool found;
};

// maps strings to enums
class EnumMapper
{
public:

	static Attacks::Enum mapAttacks(std::string input);
	static DifficultyLevel::Enum mapLevel(std::string input);
	static Classes::Enum mapClasses(std::string input);
	static Target::Enum mapTarget(std::string input);
	static Attribute::Enum mapAttribute(std::string input);
	static Mode::Enum mapMode(std::string input);
	static WeaponType::Enum mapWeaponType(std::string input);
	static RoomTypes::Enum mapRoomTypes(std::string input);

	static std::string mapRoomNames(RoomTypes::Enum input);
};

#endif // ENUMS_INCLUDE