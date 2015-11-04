#ifndef ENUMS_INCLUDE
#define ENUMS_INCLUDE

#include <string>

struct MoveState
{
	enum Enum { ARRIVED, MOVING, RESTING };
};

struct ObjectType
{
	enum Enum { ITEM, CREATURE, ATTACK, ROOM, TILE, WALL, KEY, UNKNWON };
};

struct ItemType
{
	enum Enum { WEAPON, ARMAMENT, POTION, UNKNOWN };
};

struct ArmorStatsMode
{
	enum Enum { SUM, AVG, MIN, MAX };
};

struct ArmorStatsAttributes
{
	enum Enum { ARMOR, SPEED, ACCURACY, ATTACK };
};

struct ArmamentType
{
	enum Enum { HELMET, HARNESS, GAUNTLETS, CUISSE, BOOTS, UNKNOWN };
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

struct EquipHotspots
{
	enum Enum { LEFT, RIGHT, POTION1, POTION2, POTION3, UNKNOWN };
};

struct MouseEvent
{
	enum Enum { CLICK, DRAGSTART, DRAGRELEASE, DRAGGING, UNKNOWN };
};


struct Point {
	unsigned int x;
	unsigned int y;

	Point getDirPoint(ViewingDirections::Enum dir)
	{
		Point retPoint;
		switch (dir)
		{
			case ViewingDirections::N:
				retPoint = Point{ x, y - 1 };
				break;
			case ViewingDirections::E:
				retPoint = Point{ x + 1, y };
				break;
			case ViewingDirections::S:
				retPoint = Point{ x, y + 1 };
				break;
			case ViewingDirections::W:
				retPoint = Point{ x - 1, y };
				break;
			default:
				retPoint = Point{ x, y };
				break;
		}

		return retPoint;
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
	static ArmamentType::Enum mapArmamentType(std::string input);

	static std::string mapRoomNames(RoomTypes::Enum input);
	static std::string mapArmamentTypeNames(ArmamentType::Enum input);
	static std::string mapLevelName(Classes::Enum input);
	static std::string mapTargetName(Target::Enum input);
	static std::string mapAttributeName(Attribute::Enum input);
	static std::string mapModeName(Mode::Enum input);
	static std::string mapWeaponTypeName(WeaponType::Enum input);
	static std::string mapAttackName(Attacks::Enum input);
};

#endif // ENUMS_INCLUDE