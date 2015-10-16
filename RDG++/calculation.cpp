#include "calculation.hpp"
#include "prototypes.hpp"
#include "random.hpp"
#include <iostream>

float Calculation::randomFloat(float min, float max) {
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

unsigned int Calculation::randomUInt(unsigned int min, unsigned int max) {
	return min + (rand() % (int)(max - min + 1));
}

// returns the name of a random monster, keeping them somewhat balanced 
std::string Calculation::randomMonster(RoomTemplate::MonsterProbabilities &monsterProbabilities, std::map<DifficultyLevel::Enum, std::map<std::string, unsigned int>> &monsterBalance, 
	std::map<std::string, unsigned int> &monsterBalanceOffsets, std::map<DifficultyLevel::Enum, std::list<std::string>> &monstersLeveled)
{
	// used to sum up all single possibility values from configloader
	float sum = 0;

	// used to determine which level the determined monster shall be of
	float randFloat = 0;

	// the determined, random monster level
	DifficultyLevel::Enum monsterLevel;

	// offset needed for balancing occurrence of same leveled items
	unsigned int offset = 0;

	// map of all monster counts for the chosen level
	std::map<std::string, unsigned int> balance;

	// calculate total sum to determine random interval
	sum += monsterProbabilities.no;
	sum += monsterProbabilities.easy;
	sum += monsterProbabilities.normal;
	sum += monsterProbabilities.hard;

	// get a random value within the determined interval
	randFloat = randomFloat(0.0f, sum);

	// set thresholds for level probabilities
	float noneThreshold = monsterProbabilities.no;
	float easyThreshold = noneThreshold + monsterProbabilities.easy;
	float normalThreshold = easyThreshold + monsterProbabilities.normal;
	float hardThreshold = normalThreshold + monsterProbabilities.hard;

	// determine the random monster level
	if (randFloat > 0 && randFloat <= noneThreshold) {
		monsterLevel = DifficultyLevel::UNKNOWN;
	}
	else if (randFloat > noneThreshold && randFloat <= easyThreshold) {
		monsterLevel = DifficultyLevel::EASY;
		offset = monsterBalanceOffsets.at("easy");
		balance = monsterBalance.at(DifficultyLevel::EASY);
	}
	else if (randFloat > easyThreshold && randFloat <= normalThreshold) {
		monsterLevel = DifficultyLevel::NORMAL;
		offset = monsterBalanceOffsets.at("normal");
		balance = monsterBalance.at(DifficultyLevel::NORMAL);
	}
	else if (randFloat > normalThreshold && randFloat <= hardThreshold) {
		DifficultyLevel::HARD;
		offset = monsterBalanceOffsets.at("hard");
		balance = monsterBalance.at(DifficultyLevel::HARD);
	}

	// no monster shall be placed in this room
	if (monsterLevel == DifficultyLevel::UNKNOWN) {
		return "";
	}

	// get the appriopriate list of monsters within the determined level
	std::list<std::string> &monsterLeveledList = monstersLeveled.at(monsterLevel);

	// return the name of a random monster within the list of leveled monsters keeping the count of monsters per level balanced by offset
	std::string randomMonster = "";

	std::list<std::string> mins;

	// determine min and max of all monster counts of this level
	unsigned int min = 99999;
	unsigned int max = 0;
	for (std::map<std::string, unsigned int>::iterator it = balance.begin(); it != balance.end(); it++) 
	{
		if (it->second < min)
		{
			min = it->second;
		}
		if (it->second > max) {
			max = it->second;
		}
	}

	// determine all monsters with min count
	for (std::map<std::string, unsigned int>::iterator it = balance.begin(); it != balance.end(); it++)
	{
		if (it->second <= min)
		{
			mins.push_back(it->first);
		}
	}

	if (randomMonster == "") {
		// keep monsters balanced by a max margin of offset
		if (min <= (max - offset)) {
			randomMonster = *selectRandomly(mins.begin(), mins.end());
		}
		else {
			randomMonster = *selectRandomly(monsterLeveledList.begin(), monsterLeveledList.end());
		}
	}

	return randomMonster;
}

std::map<Classes::Enum, std::map<std::string, ItemBalance>> itemsBalance;
std::map<std::string, unsigned int> itemBalanceOffsets;

// returns name and type of a random Item, keeping them somewhat balanced
ItemTypeName Calculation::randomItem(RoomTemplate::ItemProbabilities itemProbabilities, std::map<Classes::Enum, std::map<std::string, ItemBalance>> &itemsBalance,
	std::map<std::string, unsigned int> &itemBalanceOffsets, std::map<Classes::Enum, std::list<std::pair<std::string, ItemType::Enum>>> &itemsClassList)
{

	// used to sum up all single possibility values for all items from configloader
	float sum = 0;

	// used to determine which itemClass the determined item shall be of
	float randFloat = 0;

	// the determined, random itemClass
	Classes::Enum itemClass;

	// offset needed for balancing occurrence of same leveled items
	unsigned int offset = 0;

	// map of all item counts for the chosen level
	std::map<std::string, ItemBalance> balance;

	// calculate total some to determine random interval
	sum += itemProbabilities.none;
	sum += itemProbabilities.weak;
	sum += itemProbabilities.medium;
	sum += itemProbabilities.strong;

	// get a random value within the determined interval = sum of all probability values
	randFloat = randomFloat(0.0f, sum);

	// set thresholds for itemClass probabilities
	float noneThreshold = itemProbabilities.none;
	float weakThreshold = noneThreshold + itemProbabilities.weak;
	float mediumThreshold = weakThreshold + itemProbabilities.medium;
	float strongThreshold = mediumThreshold + itemProbabilities.strong;

	/* determine the random itemClass */
	if (randFloat > 0 && randFloat <= noneThreshold) {
		itemClass = Classes::NONE;
	}
	else if (randFloat > noneThreshold && randFloat <= weakThreshold) {
		itemClass = Classes::WEAK;
		offset = itemBalanceOffsets.at("weak");
		balance = itemsBalance.at(Classes::WEAK);
	}
	else if (randFloat > weakThreshold && randFloat <= mediumThreshold) {
		itemClass = Classes::MEDIUM;
		offset = itemBalanceOffsets.at("medium");
		balance = itemsBalance.at(Classes::MEDIUM);
	}
	else if (randFloat > mediumThreshold && randFloat <= strongThreshold) {
		itemClass = Classes::STRONG;
		offset = itemBalanceOffsets.at("strong");
		balance = itemsBalance.at(Classes::STRONG);
	}

	// no item shall be placed in this room
	if (itemClass == Classes::NONE) {
		return ItemTypeName{ "", ItemType::UNKNOWN};
	}

	// get the appriopriate list of items within the determined class
	std::list<std::pair<std::string, ItemType::Enum>> &itemClassList = itemsClassList.at(itemClass);

	// return the name of a random item within the list of leveled items keeping the items per level balanced
	ItemTypeName randomItem;

	std::list<ItemTypeName> mins;

	// determine min and max of all monster counts of this level
	unsigned int min = 99999;
	unsigned int max = 0;
	for (std::map<std::string, ItemBalance>::iterator it = balance.begin(); it != balance.end(); it++)
	{
		if (it->second.balanceCount < min)
		{
			min = it->second.balanceCount;
		}
		if (it->second.balanceCount > max) {
			max = it->second.balanceCount;
		}
	}

	// determine all items with min count
	for (std::map<std::string, ItemBalance>::iterator it = balance.begin(); it != balance.end(); it++)
	{
		if (it->second.balanceCount <= min)
		{
			mins.push_back(ItemTypeName{ it->first, it->second.itemType });
		}
	}

	if (randomItem.itemName == "") {
		// keep items balanced by a max margin of offset
		if (min <= (max - offset)) {
			randomItem = *selectRandomly(mins.begin(), mins.end());
		}
		else {
			std::pair<std::string, ItemType::Enum> randPair = *selectRandomly(itemClassList.begin(), itemClassList.end());
			randomItem = ItemTypeName{randPair.first, randPair.second};
		}
	}

	return randomItem;
}

// returns a random monster attack
Attacks::Enum Calculation::randomAttackType() {

	return static_cast<Attacks::Enum>(randomUInt(3)); // 0: TORSO, 1: HEAD, 2: ARMS, 3: LEGS
}

// returns Point for a random Room on the map, excluding the treasure Chamber
Point Calculation::randomRoom(unsigned int roomsX, unsigned int roomsY) {

	unsigned int middleX = (unsigned int) roomsX / 2;
	unsigned int middleY = (unsigned int) roomsY / 2;
	unsigned int randX = 0;
	unsigned int randY = 0;

	do {
		randX = randomUInt(roomsX);
		randY = randomUInt(roomsY);
	} while (randX == middleX && randY == middleY);

	Point randRoom{ randX, randY };

	return randRoom;
}

// returns random empty tile in tilemap
Point Calculation::randomTile(std::vector<std::vector<std::shared_ptr<RenderableObject>>> &tilemap, unsigned int width, unsigned int height, unsigned int maxIterations) {

	unsigned int randX = 0;
	unsigned int randY = 0;
	bool found = false;
	unsigned int ctr = 0;

	do {
		randX = randomUInt(width);
		randY = randomUInt(height);

		if (tilemap[randX][randY] == nullptr) {
			found = true;
		}
		ctr++;
	} while (found == false && ctr <= maxIterations);

	Point randTile{ randX, randY };

	return randTile;
}