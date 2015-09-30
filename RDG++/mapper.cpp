#include "enums.hpp"
#include "exceptions.hpp"

DoorPositions EnumMapper::mapDoorPositions(std::string input)
{
	if (input == "N")
	{
		return DoorPositions::North;
	}
	else if (input == "E")
	{
		return DoorPositions::East;
	}
	else if (input == "S")
	{
		return DoorPositions::South;
	}
	else if (input == "W")
	{
		return DoorPositions::West;
	}
	else
	{
		throw EnumMappingException(input, "DoorPositions");
	}
}

MonsterProbabilities EnumMapper::mapMonsterProbabilities(std::string input)
{
	if (input == "no")
	{
		return MonsterProbabilities::No;
	}
	else if (input == "easy")
	{
		return MonsterProbabilities::Easy;
	}
	else if (input == "normal")
	{
		return MonsterProbabilities::Normal;
	}
	else if (input == "hard")
	{
		return MonsterProbabilities::Hard;
	}
	else
	{
		throw EnumMappingException(input, "MonsterProbabilities");
	}
}

DifficultyLevel::Level EnumMapper::mapLevel(std::string input)
{
	if (input == "easy")
	{
		return DifficultyLevel::Level::Easy;
	}
	else if (input == "normal")
	{
		return DifficultyLevel::Level::Normal;
	}
	else if (input == "hard")
	{
		return DifficultyLevel::Level::Hard;
	}
	else
	{
		throw EnumMappingException(input, "Level");
	}
}

Classes EnumMapper::mapClasses(std::string input)
{
	if (input == "-" || input == "none")
	{
		return Classes::None;
	}
	else if (input == "weak")
	{
		return Classes::Weak;
	}
	else if (input == "medium")
	{
		return Classes::Medium;
	}
	else if (input == "strong")
	{
		return Classes::Strong;
	}
	else
	{
		throw EnumMappingException(input, "ItemClass");
	}
}

Target EnumMapper::mapTarget(std::string input)
{
	if (input == "self")
	{
		return Target::Self;
	}
	else if (input == "opponent")
	{
		return Target::Opponent;
	}
	else
	{
		throw EnumMappingException(input, "Target");
	}
}

Attribute EnumMapper::mapAttribute(std::string input)
{
	if (input == "hp")
	{
		return Attribute::Hp;
	}
	else if (input == "speed")
	{
		return Attribute::Speed;
	}
	else if (input == "accuracy")
	{
		return Attribute::Accuracy;
	}
	else if (input == "strength")
	{
		return Attribute::Strength;
	}
	else
	{
		throw EnumMappingException(input, "Effect");
	}
}

Mode EnumMapper::mapMode(std::string input)
{
	if (input == "x")
	{
		return Mode::Cure;
	}
	else if (input == "<")
	{
		return Mode::TemporaryDecrease;
	}
	else if (input == "-")
	{
		return Mode::IncrementalDecrease;
	}
	else if (input == ">")
	{
		return Mode::TemporaryIncrease;
	}
	else if (input == "+")
	{
		return Mode::IncrementalIncrease;
	}
	else
	{
		throw EnumMappingException(input, "Mode");
	}
}