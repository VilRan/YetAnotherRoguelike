#include "Point.h"
#include <math.h>


Point::Point()
{
	X = 0;
	Y = 0;
}

Point::Point(int x, int y)
{
	X = x;
	Y = y;
}

Point::~Point()
{
}

double Point::GetDistance(Point other)
{
	int a = other.X - X;
	int b = other.Y - Y;
	return sqrt(a * a + b * b);
}
