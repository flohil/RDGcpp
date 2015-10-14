#ifndef ENUMS_INCLUDE
#define ENUMS_INCLUDE

#include <string>

struct ObjectType
{
	enum Enum { ITEM, CREATURE, ATTACK, ROOM, TILE, WALL };
};

struct ItemType
{
	enum Enum { WEAPON, ARMAMENT, POTION };
};

struct ViewingDirections
{
	enum Enum { N, E, S, W };
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