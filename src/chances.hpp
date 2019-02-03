#ifndef CALCULATION_INCLUDE
#define CALCULATION_INCLUDE

#include <vector>
#include <memory>
#include "enums.hpp"
#include "gameObjects.hpp"
#include "prototypes.hpp"

class Chances
{
public:

	static float randomFloat(float min, float max);
	static unsigned int randomUInt(unsigned int min, unsigned int max);
	static unsigned int randomUInt(unsigned int max) { return randomUInt(0, max); };
	static FoundPoint randomFreeTile(std::vector<std::vector<std::shared_ptr<RenderableObject>>> &tilemap, unsigned int maxIterations);
	static Point randomRoom(unsigned int roomsX, unsigned int roomsY);
	static Attacks::Enum Chances::randomAttackType();
	static ViewingDirections::Enum Chances::randomViewingDirection();
	static std::string randomMonster(RoomTemplate::MonsterProbabilities &monsterProbabilities, std::map<DifficultyLevel::Enum, 
		std::map<std::string, unsigned int>> &monsterBalance, std::map<std::string, unsigned int> &monsterBalanceOffsets, std::map<DifficultyLevel::Enum, std::list<std::string>> &monstersLeveled);
	static ItemTypeName randomItem(RoomTemplate::ItemProbabilities itemProbabilities, std::map<Classes::Enum, std::map<std::string, ItemBalance>> &itemsBalance,
		std::map<std::string, unsigned int> &itemBalanceOffsets, std::map<Classes::Enum, std::list<std::pair<std::string, ItemType::Enum>>> &itemsClassList);
};

#endif // CALCULATION_INCLUDE