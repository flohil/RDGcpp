#pragma once
class MinAvgMax
{
public:
	MinAvgMax(float min, float max, float base);
	MinAvgMax(float min, float max);
	~MinAvgMax();

	float min;
	float avg;
	float max;
};

