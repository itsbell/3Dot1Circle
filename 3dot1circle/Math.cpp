//Math.cpp
#include "Math.h"

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
