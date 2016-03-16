#pragma once
#include <string>

using namespace std;

enum TextAlign
{
	Left, Center, Right
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	static void DrawBox(int x, int y, int width, int height);
	static void DrawBox(int x, int y, int width, int height, char c);
	static void Print(const char * str, int x, int y, TextAlign alignment = Left);
	static void Print(string str, int x, int y, TextAlign alignment = Left);
	static void MoveCursor(int x, int y);
	static void UseColor(int foreground, int background);
	static void InitializeColors();
};

