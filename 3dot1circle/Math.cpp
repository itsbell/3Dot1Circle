//Math.cpp
#include "Math.h"
#include <random>

Math::Math()
{
}

Math::~Math()
{
}

double Math::Area(CPoint a, CPoint b, CPoint c)
{
	return abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0;
}

CPoint Math::Center(CPoint a, CPoint b)
{
	return CPoint((a.x + b.x) / 2, (a.y + b.y) / 2);
}

double Math::Distance(CPoint a, CPoint b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

int Math::Random(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);

	return distrib(gen);
}
