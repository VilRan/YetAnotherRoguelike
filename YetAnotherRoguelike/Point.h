#pragma once
class Point
{
public:
	int X, Y;

	Point();
	Point(int x, int y);
	~Point();

	double GetDistance(Point other);
};

