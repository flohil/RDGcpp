#include "minAvgMax.hpp"


MinAvgMax::MinAvgMax(float min_, float max_, float base_)
{
	min = min_ * base_;
	max = max_ * base_;
	avg = min + (max - min) / 2;
}

MinAvgMax::MinAvgMax(float min_, float max_)
{
	min = min_;
	max = max_;
	avg = min_ + (max_ - min_) / 2;
}

MinAvgMax::~MinAvgMax()
{
}
