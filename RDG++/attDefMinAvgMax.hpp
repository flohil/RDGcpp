#pragma once

#include "minAvgMax.hpp"
class AttDefMinAvgMax
{
public:
	AttDefMinAvgMax();
	AttDefMinAvgMax(MinAvgMax attacker, MinAvgMax defender);
	~AttDefMinAvgMax();

	MinAvgMax attacker;
	MinAvgMax defender;
};

