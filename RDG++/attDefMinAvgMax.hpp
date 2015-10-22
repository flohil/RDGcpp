#ifndef ATT_DEF_MIN_AVG_MAX_INCLUDE
#define ATT_DEF_MIN_AVG_MAX_INCLUDE

#include "minAvgMax.hpp"
class AttDefMinAvgMax
{
public:
	AttDefMinAvgMax();
	AttDefMinAvgMax(MinAvgMax attacker, MinAvgMax defender);
	~AttDefMinAvgMax();

	MinAvgMax* attacker;
	MinAvgMax* defender;
}

#endif /* ATT_DEF_MIN_AVG_MAX_INCLUDE */

