#include "enums.hpp"
#include <iostream>

Attacks::Enum EnumMapper::mapAttacks(std::string input)
{
	if (input == "Torso")
	{
		return Attacks::Enum::TORSO;
	}
	else if (input == "Head")
	{
		return Attacks::Enum::HEAD;
	}
	else if (input == "Arms")
	{
		return Attacks::Enum::ARMS;
	}
	else if (input == "Legs")
	{
		return Attacks::Enum::LEGS;
	}
	else if (input == "Parry")
	{
		return Attacks::Enum::PARRY;
	}
	else if (input == "Set")
	{
		return Attacks::Enum::SET;
	}
	else if (input == "Potion")
	{
		return Attacks::Enum::POTION;
	}
	else
	{
		std::cerr << "Unknown input for Attacks::Enum: " << input << std::endl;
		return Attacks::Enum::UNKNOWN;
	}
}

DifficultyLevel::Enum EnumMapper::mapLevel(std::string input)
{
	if (input == "easy")
	{
		return DifficultyLevel::Enum::EASY;
	}
	else if (input == "normal")
	{
		return DifficultyLevel::Enum::NORMAL;
	}
	else if (input == "hard")
	{
		return DifficultyLevel::Enum::HARD;
	}
	else
	{
		std::cerr << "Unknown input for DifficultyLevel::Enum: " << input << std::endl;
		return DifficultyLevel::Enum::UNKNOWN;
	}
}

Classes::Enum EnumMapper::mapClasses(std::string input)
{
	if (input == "-" || input == "none")
	{
		return Classes::Enum::NONE;
	}
	else if (input == "weak")
	{
		return Classes::Enum::WEAK;
	}
	else if (input == "medium")
	{
		return Classes::Enum::MEDIUM;
	}
	else if (input == "strong")
	{
		return Classes::Enum::STRONG;
	}
	else
	{
		std::cerr << "Unknown input for Classes::Enum: " << input << std::endl;
		return Classes::Enum::UNKNOWN;
	}
}

Target::Enum EnumMapper::mapTarget(std::string input)
{
	if (input == "self")
	{
		return Target::Enum::SELF;
	}
	else if (input == "opponent")
	{
		return Target::Enum::OPPONENT;
	}
	else
	{
		std::cerr << "Unknown input for Target::Enum: " << input << std::endl;
		return Target::Enum::UNKNOWN;
	}
}

Attribute::Enum EnumMapper::mapAttribute(std::string input)
{
	if (input == "hp")
	{
		return Attribute::Enum::HP;
	}
	else if (input == "speed")
	{
		return Attribute::Enum::SPEED;
	}
	else if (input == "accuracy")
	{
		return Attribute::Enum::ACCURACY;
	}
	else if (input == "strength")
	{
		return Attribute::Enum::STRENGTH;
	}
	else
	{
		std::cerr << "Unknown input for Attribute::Enum: " << input << std::endl;
		return Attribute::Enum::UNKNOWN;
	}
}

Mode::Enum EnumMapper::mapMode(std::string input)
{
	if (input == "x")
	{
		return Mode::Enum::CURE;
	}
	else if (input == "<")
	{
		return Mode::Enum::TEMPORARY_DECREASE;
	}
	else if (input == "-")
	{
		return Mode::Enum::INCREMENTAL_DECREASE;
	}
	else if (input == ">")
	{
		return Mode::Enum::TEMPORARY_INCREASE;
	}
	else if (input == "+")
	{
		return Mode::Enum::INCREMENTAL_INCREASE;
	}
	else
	{
		std::cerr << "Unknown input for Mode::Enum: " << input << std::endl;
		return Mode::Enum::UNKNOWN;
	}
}

WeaponType::Enum EnumMapper::mapWeaponType(std::string input)
{
	if (input == "single-hand")
	{
		return WeaponType::Enum::SINGLE;
	}
	else if (input == "two-hand")
	{
		return WeaponType::Enum::DOUBLE;
	}
	else
	{
		std::cerr << "Unknown input for WeaponType::Enum: " << input << std::endl;
		return WeaponType::Enum::UNKNOWN;
	}
}