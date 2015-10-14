#include "enums.hpp"
#include <iostream>
#include "easylogging++.hpp"

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
		LOG(ERROR) << "Unknown input for Attacks::Enum: " << input;
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
		LOG(ERROR) << "Unknown input for DifficultyLevel::Enum: " << input;
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
		LOG(ERROR) << "Unknown input for Classes::Enum: " << input;
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
		LOG(ERROR) << "Unknown input for Target::Enum: " << input;
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
		LOG(ERROR) << "Unknown input for Attribute::Enum: " << input;
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
		LOG(ERROR) << "Unknown input for Mode::Enum: " << input;
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
		LOG(ERROR) << "Unknown input for WeaponType::Enum: " << input;
		return WeaponType::Enum::UNKNOWN;
	}
}

RoomTypes::Enum EnumMapper::mapRoomTypes(std::string input)
{
	if (input == "Dead End")
	{
		return RoomTypes::DEADEND;
	}
	if (input == "Hallway")
	{
		return RoomTypes::HALLWAY;
	}
	if (input == "Turn")
	{
		return RoomTypes::TURN;
	}
	if (input == "T-Junction")
	{
		return RoomTypes::TJUNCTION;
	}
	if (input == "Junction")
	{
		return RoomTypes::JUNCTION;
	}
	if (input == "Treasure Chamber")
	{
		return RoomTypes::TREASURECHAMBER;
	}
	else
	{
		LOG(ERROR) << "Unknown input for RoomTypes::Enum: " << input;
		return RoomTypes::Enum::UNKNOWN;
	}
}

std::string EnumMapper::mapRoomNames(RoomTypes::Enum input)
{
	switch (input)
	{
	case RoomTypes::DEADEND:
		return "Dead End";
	case RoomTypes::HALLWAY:
		return "Hallway";
	case RoomTypes::JUNCTION:
		return "Junction";
	case RoomTypes::TJUNCTION:
		return "T-Junction";
	case RoomTypes::TREASURECHAMBER:
		return "Treasure Chamber";
	case RoomTypes::TURN:
		return "Turn";
	case RoomTypes::UNKNOWN:
		LOG(ERROR) << "Unknown input for RoomTypes::Enum: " << input;
		return "Unknown";
	default:
		return "Unknown";
	}
}