#ifndef ENUMS_INCLUDE
#define ENUMS_INCLUDE

#include <string>

enum DoorPositions { North, East, South, West };
enum MonsterProbabilities { No, Easy, Normal, Hard };
namespace DifficultyLevel
{
	enum Level { Easy, Normal, Hard };
}
enum Classes { None, Weak, Medium, Strong };
enum Target { Self, Opponent };
enum Attribute { Hp, Speed, Accuracy, Strength };
enum Mode { Cure, TemporaryDecrease, IncrementalDecrease, TemporaryIncrease, IncrementalIncrease };
enum RoomTypes { Deadend, Hallway, Turn, Tjunction, Junction, Treasurechamber };
enum WeaponType { Single, Double };

// maps strings to enums
class EnumMapper
{
public:

	static DoorPositions mapDoorPositions(std::string input);
	static MonsterProbabilities mapMonsterProbabilities(std::string input);
	static DifficultyLevel::Level mapLevel(std::string input);
	static Classes mapClasses(std::string input);
	static Target mapTarget(std::string input);
	static Attribute mapAttribute(std::string input);
	static Mode mapMode(std::string input);
	static WeaponType mapWeaponType(std::string input);
};

#endif // ENUMS_INCLUDE